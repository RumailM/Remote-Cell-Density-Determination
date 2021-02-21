// Communication
#include "PubSubClient.h" // Connect and publish to the MQTT broker
#include "WiFi.h" // Enables the ESP32 to connect to the local network (via WiFi)
#include <ArduinoJson.h> // Enables JSON serialization and deserialization

// Sampling
#include <stdint.h>
#include <stdio.h>

// Wireless
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

// Communication
String macAddress;
const char* clientID = "";
int identifier;
bool flag_handshake = false, flag_identification = false, flag_start = false;
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient); 

// Functions

///////////////////   CALLBACKS    ///////////////

void callback_login_response(char* topic, byte* payload, unsigned int length);
void callback_experiment_start(char* topic, byte* payload, unsigned int length);
void callback_experiment_stop(char* topic, byte* payload, unsigned int length);
void callback_default(char* topic, byte* payload, unsigned int length);
void callback(char* topic, byte* payload, unsigned int length);

///////////////////   HANDSHAKING    ///////////////

void connect_wifi();
void connect_MQTT();
void identify_handshake();