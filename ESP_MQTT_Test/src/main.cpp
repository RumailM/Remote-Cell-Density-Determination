#include <Arduino.h>
#include "PubSubClient.h" // Connect and publish to the MQTT broker
#include "WiFi.h" // Enables the ESP32 to connect to the local network (via WiFi)
#include <ArduinoJson.h> // Enables JSON serialization and deserialization

// WiFi
const char* ssid = "NameOfNetwork";                     // Raspberry Pi network SSID
const char* wifi_password = "AardvarkBadgerHedgehog";   // Raspberry Pi network password
byte mac_address[6];                                    // ESP32 MAC Address
String macAddress;

// MQTT
const char* mqtt_server = "192.168.4.1";  // IP of the MQTT broker
const char* mqtt_username = "smart"; // MQTT username
const char* mqtt_password = "clamp"; // MQTT password
const char* clientID = ""; // MQTT client ID
int identifier;

// MQTT Topics
const char* topic_login = "lab/control/login";
const char* topic_login_response = "lab/control/loginResponse";
const char* topic_experiment_start = "lab/control/experimentStart";
const char* topic_experiment_stop = "lab/control/experimentStop";
const char* topic_experiment_data = "lab/data";

// JSON Message
char concatenation [50];

// Flags
bool flag_identification = false, flag_start = false;

// Timing
const unsigned long period = 2000;
unsigned long start_millis, current_millis;

// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
// 1883 is the listener port for the Broker
PubSubClient client(mqtt_server, 1883, wifiClient); 

void callback(char* topic, byte* payload, unsigned int length);

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

    WiFi.macAddress(mac_address);
    macAddress = WiFi.macAddress();

    // Debugging - Output the IP Address and MAC Address
    Serial.println("WiFi connected");

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.print("MAC address: ");
    Serial.println(macAddress.c_str());
    
    sprintf(concatenation, "{\"MAC\":\"%s\"}", macAddress.c_str());
    Serial.println(concatenation);
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
    bool flag_handshake = false;
    while (!flag_handshake)
    {
        if (client.subscribe(topic_login_response))
        {
            Serial.println("Subscribed to login response topic!");
        }
        else
        {
            Serial.println("Failed to subscribe to login response topic.");
        }

        if (client.publish(topic_login, concatenation))
        {
            Serial.println("MAC sent!");
            flag_handshake = true;
        }
        else
        {
            Serial.println("String failed to send. Reconnecting to MQTT Broker and trying again");
            connect_MQTT();
        }
    }

    Serial.println("I am done.");
}

void callback_login_response(char* topic, byte* payload, unsigned int length)
{
    Serial.println("Response received.");
    
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

        // Check identifier
        if (identifier == doc["id"])
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
        if (identifier == doc["id"])
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

        Serial.println("Experiment successfully started!");
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

void callback(char* topic, byte* payload, unsigned int length)
{
    Serial.println("Hello World!");
}

// // Callback function for subscribed topics
// void callback(char* topic, byte* payload, unsigned int length)
// {
//     Serial.print("Message arrived in topic: ");
//     Serial.println(topic);

//     // Topic Callback Functions
//     if (strcmp(topic, topic_login_response) == 0)
//     {
//         callback_login_response(topic, payload, length);
//     }
//     else if (strcmp(topic, topic_experiment_start) == 0)
//     {
//         callback_experiment_start(topic, payload, length);
//     }
//     else if (strcmp(topic, topic_experiment_stop) == 0)
//     {
//         callback_experiment_stop(topic, payload, length);
//     }
//     else
//     {
//         callback_default(topic, payload, length);
//     }
// }

void setup() 
{
    Serial.begin(115200);
    connect_wifi();
    connect_MQTT();
    identify_handshake();
    Serial.setTimeout(2000);
    start_millis = millis();
}

void loop()
{
    current_millis = millis();
    if (flag_identification && flag_start && current_millis - start_millis >= period)
    {
        Serial.print("Identifier: ");
        Serial.println(identifier);
        if (client.publish(topic_experiment_data, "Hello World!"))
        {
            Serial.println("Data sent!");
        }
        else
        {
            Serial.println("Data failed to send. Reconnecting to MQTT Broker and trying again");
            connect_MQTT();
        }
    }
}