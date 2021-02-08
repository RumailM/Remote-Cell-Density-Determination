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
const char* mqtt_server = "192.168.0.110";  // IP of the MQTT broker
const char* mqtt_username = "smart"; // MQTT username
const char* mqtt_password = "clamp"; // MQTT password
const char* clientID = ""; // MQTT client ID
int identifier;

// MQTT Topics
const char* topic_login = "lab/control/login";
const char* topic_login_response = "lab/control/loginResponse";
const char* topic_experiment_toggle = "lab/control/experimentToggle";
const char* topic_experiment_data = "lab/data";

// JSON Message
char concatenation [50];

// Flags
bool flag_identification = false;

// Timing
const unsigned long period = 2000;
unsigned long start_millis, current_millis;

// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
// 1883 is the listener port for the Broker
PubSubClient client(mqtt_server, 1883, wifiClient); 

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
    }
    else
    {
        Serial.println("Connection to MQTT Broker failed...");
    }
}

// Callback function for subscribed topics
void callback(char* topic, byte* payload, unsigned int length)
{
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);

    Serial.print("Message:");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }

    Serial.println();
    Serial.println("-----------------------");

    // Login Topic Callback
    if (strcmp(topic, topic_login_response) == 0)
    {
        // Assign unique identifier and unsub from login topic
        identifier = (int)(char)payload[1];
        Serial.println(identifier);
        if (client.unsubscribe(topic_login_response))
        {
            Serial.println("Unsubscribed from login response topic!");
        }
        else
        {
            Serial.println("Failed to unsubscribe from login response topic.");
        }

        // Sub to experimentToggle topic
        if (client.subscribe(topic_experiment_toggle))
        {
            Serial.println("Subscribed to experimentToggle topic!");
        }
        else
        {
            Serial.println("Failed to subscribe to experimentToggle topic.");
        }

        // Raise identification flag
        flag_identification = true;
    }
}

// Custom function to obtain ID from Raspberry Pi
void identify_handshake()
{
    client.setCallback(callback);
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
    client.setCallback(callback);
}


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
    if (flag_identification && current_millis - start_millis >= period)
    {
        Serial.print("Identifier: ");
        Serial.println(identifier);
        if (client.publish(topic_experiment_toggle, "Hello World!"))
        {
            Serial.println("Hello sent!");
        }
        else
        {
            Serial.println("Hello failed to send. Reconnecting to MQTT Broker and trying again");
            connect_MQTT();
        }
    }
}