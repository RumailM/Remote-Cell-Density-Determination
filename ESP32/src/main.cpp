
///////////////////   LIBRARIES    ///////////////

// Communication
#include "PubSubClient.h" // Connect and publish to the MQTT broker
#include "WiFi.h" // Enables the ESP32 to connect to the local network (via WiFi)
#include <ArduinoJson.h> // Enables JSON serialization and deserialization

// Sampling
#include <stdint.h>
#include <stdio.h>

#include <Smartclamp_LED.h>
#include <Smartclamp_Serial.h>
#include <Smartclamp_AS7341.h>

///////////////////   CONSTANTS    ///////////////

// Sampling
const String SMARTCLAMP_VERSION = "0.15";
const unsigned long SERIAL_DELAY = 227;
const unsigned long TARGET_PERIOD = 500;
const unsigned long READING_PERIOD = TARGET_PERIOD - SERIAL_DELAY;
const unsigned int AGC_FREQUENCY = 20;

// Communication
const char* ssid = "NameOfNetwork";                     // Raspberry Pi network SSID
const char* wifi_password = "AardvarkBadgerHedgehog";   // Raspberry Pi network password
const char* mqtt_server = "192.168.4.1";  // IP of the MQTT broker
const char* mqtt_username = "smart"; // MQTT username
const char* mqtt_password = "clamp"; // MQTT password
const char* topic_login = "lab/control/login";
const char* topic_login_response = "lab/control/loginResponse";
const char* topic_experiment_start = "lab/control/experimentStart";
const char* topic_experiment_stop = "lab/control/experimentStop";
const char* topic_experiment_data = "lab/data";
const unsigned long period = 2000;

///////////////////   GLOBAL VARIABLES    ///////////////

// Sampling
Smartclamp_AS7341 as7341;
unsigned long lastMsecs = millis();
bool rawCountsMode = true;
unsigned int cnt_agc = 0;

// Communication
String macAddress;
const char* clientID = "";
int identifier;
bool flag_handshake = false, flag_identification = false, flag_start = false;
unsigned long start_millis, current_millis;
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient); 

///////////////////   CALLBACKS    ///////////////

void callback_login_response(char* topic, byte* payload, unsigned int length)
{
    flag_handshake = true;
    Serial.println("Login Response received.");
    
    StaticJsonDocument<256> doc;
    DeserializationError err = deserializeJson(doc, payload, length);

    if (err)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(err.f_str());
        return;
    }
    
    const char* mac = doc["MAC"];

    if (strcmp(macAddress.c_str(), mac) == 0)
    {
        // Assign unique identifier and unsub from login topic
        identifier = doc["id"];
        Serial.println(identifier);
        if (client.unsubscribe(topic_login_response))
        {
            Serial.println("Unsubscribed from loginResponse topic!");
        }
        else
        {
            Serial.println("Failed to unsubscribe from loginResponse topic.");
        }

        // Sub to experimentToggle topic
        if (client.subscribe(topic_experiment_start))
        {
            Serial.println("Subscribed to experimentStart topic!");
        }
        else
        {
            Serial.println("Failed to subscribe to experimentStart topic.");
        }

        // Raise identification flag
        flag_identification = true;
    }
}

void callback_experiment_start(char* topic, byte* payload, unsigned int length)
{
    if (flag_identification)
    {
        StaticJsonDocument<256> doc;
        DeserializationError err = deserializeJson(doc, payload, length);

        if (err)
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(err.f_str());
            return;
        }

        Serial.println(identifier);

        // Check identifier
        if (identifier == (int) doc["id"])
        {
            // Unsub from experiment start topic
            if (client.unsubscribe(topic_experiment_start))
            {
                Serial.println("Unsubscribed from experimentStart topic!");
            }
            else
            {
                Serial.println("Failed to unsubscribe from experimentStart topic.");
            }

            // Sub to experiment stop topic
            if (client.subscribe(topic_experiment_stop))
            {
                Serial.println("Subscribed to experimentStop topic!");
            }
            else
            {
                Serial.println("Failed to subscribe to experimentStop topic.");
            }

            // Raise flag_start
            flag_start = true;
        }

        Serial.println("Experiment successfully started!");
    }
}

void callback_experiment_stop(char* topic, byte* payload, unsigned int length)
{
    if (flag_start)
    {
        StaticJsonDocument<256> doc;
        DeserializationError err = deserializeJson(doc, payload, length);

        if (err)
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(err.f_str());
            return;
        }

        // Check identifier
        if (identifier == (int) doc["id"])
        {
            // Unsub from experiment stop topic
            if (client.unsubscribe(topic_experiment_stop))
            {
                Serial.println("Unsubscribed from experimentStop topic!");
            }
            else
            {
                Serial.println("Failed to unsubscribe from experimentStop topic.");
            }

            // Sub to experiment start topic
            if (client.subscribe(topic_experiment_start))
            {
                Serial.println("Subscribed to experimentStart topic!");
            }
            else
            {
                Serial.println("Failed to subscribe to experimentStart topic.");
            }

            // Raise flag_start
            flag_start = false;
        }

        Serial.println("Experiment successfully stopped!");
    }
}

void callback_default(char* topic, byte* payload, unsigned int length)
{
    StaticJsonDocument<256> doc;
    DeserializationError err = deserializeJson(doc, payload, length);

    if (err)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(err.f_str());
        return;
    }
}

// Callback function for subscribed topics
void callback(char* topic, byte* payload, unsigned int length)
{
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);

    // Topic Callback Functions
    if (strcmp(topic, topic_login_response) == 0)
    {
        callback_login_response(topic, payload, length);
    }
    else if (strcmp(topic, topic_experiment_start) == 0)
    {
        callback_experiment_start(topic, payload, length);
    }
    else if (strcmp(topic, topic_experiment_stop) == 0)
    {
        callback_experiment_stop(topic, payload, length);
    }
    else
    {
        callback_default(topic, payload, length);
    }
}

///////////////////   HANDSHAKING    ///////////////

// Custom function to connect to WiFi
void connect_wifi()
{
    Serial.print("Connecting to ");
    Serial.println(ssid);

    // Connect to the WiFi
    WiFi.begin(ssid, wifi_password);

    // Wait until the connection has been confirmed before continuing
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    // Get MAC Address
    macAddress = WiFi.macAddress();

    // Debugging - Output the IP Address and MAC Address
    Serial.println("WiFi connected");

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.print("MAC address: ");
    Serial.println(macAddress.c_str());
}

// Custom function to connect to the MQTT broker via WiFi
void connect_MQTT()
{
    // Connect to MQTT Broker
    // client.connect returns a boolean value to let us know if the connection was successful.
    // If the connection is failing, make sure you are using the correct MQTT Username and Password
    if (client.connect(clientID, mqtt_username, mqtt_password))
    {
        Serial.println("Connected to MQTT Broker!");
        client.setCallback(callback);
    }
    else
    {
        Serial.println("Connection to MQTT Broker failed...");
    }
}


// Custom function to obtain ID from Raspberry Pi
void identify_handshake()
{

    StaticJsonDocument<256> doc;
    doc["MAC"] = macAddress.c_str();

    char buffer[256];
    size_t n = serializeJson(doc, buffer);  
    
    if (client.subscribe(topic_login_response))
    {
        Serial.println("Subscribed to login response topic!");
    }
    else
    {
        Serial.println("Failed to subscribe to login response topic.");
    }

    if (client.publish(topic_login, buffer, n))
    {
        Serial.println("MAC sent!");
    }
    else
    {
        Serial.println("String failed to send. Reconnecting to MQTT Broker and trying again");
        connect_MQTT();
    }
}

///////////////////   SETUP    ///////////////

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(1);
    }
    Serial.println("START");
    as7341.initializeSensor();

    identifier = -99;

    connect_wifi();
    connect_MQTT();

    start_millis = millis();

    // Setup LED PWM Signal.
    setupLED();
}

///////////////////   LOOP    ///////////////

// if (current_millis - start_millis >= period)

int count = 0;

void loop(void)
{
    client.loop();

    current_millis = millis();
    if (current_millis - lastMsecs > 50)
    {
        Serial.print("Main Loop. Identifier: ");
        Serial.print(identifier);
        Serial.print(", flag_identification: ");
        Serial.print(flag_identification);
        Serial.print(", flag_start: ");
        Serial.println(flag_start);

        if(!flag_handshake)
        {
            identify_handshake();
        }
        else if (flag_identification && flag_start)
        {
            uint16_t readings[12];
            float counts[12];
            
            if (cnt_agc > AGC_FREQUENCY)
            {
                as7341.automaticGainContol();
            }
            cnt_agc++;

            if (!as7341.readHighChannels(readings))
            {
                Serial.println("ERROR: Couldn't read all channels!");
                return;
            }

            if (!rawCountsMode)
            {
                for (uint8_t i = 0; i < 12; i++)
                {
                    if (i == 4 || i == 5)
                        continue;
                    // we skip the first set of duplicate clear/NIR readings
                    // (indices 4 and 5)
                    counts[i] = as7341.toBasicCounts(readings[i]);
                }
            }

            if (!rawCountsMode)
                serialHighCounts(Serial, counts);
            else
                serialHighRaw(Serial, readings);

            as7341.printParameters(Serial);

            StaticJsonDocument<256> doc;
            doc["id"] = identifier;
            doc["timestamp"] = current_millis;
            
            JsonArray data = doc.createNestedArray("data");
            
            for (int i = 0; i < 12; i++)
            {
                data.add(readings[i]);
            }
            data.add(count++);

            Serial.print("Count: ");
            Serial.println(count);

            char buffer[256];
            size_t n = serializeJson(doc, buffer);  
            
            if (client.publish(topic_experiment_data, buffer, n))
            {
                Serial.println("Data sent!");
            }
            else
            {
                Serial.println("Data failed to send. Reconnecting to MQTT Broker and trying again");
                connect_MQTT();
            }
        }
        start_millis = current_millis;
        lastMsecs = millis();
    }
    else
    {
        read_SERIAL(as7341);
    }
}