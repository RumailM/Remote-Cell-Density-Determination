#ifndef _SMARTCLAMP_COMMUNICATION_H
#define _SMARTCLAMP_COMMUNICATION_H

#include "PubSubClient.h" // Connect and publish to the MQTT broker
#include "WiFi.h" // Enables the ESP32 to connect to the local network (via WiFi)
#include <ArduinoJson.h> // Enables JSON serialization and deserialization
#include <sstream>
#include <string>

#include <Smartclamp_AS7341.h>

class Smartclamp_Communication
{
public:

    // Constants
    String macAddress;
    const char* clientID = "";
    int identifier;
    bool flag_handshake = false, flag_identification = false, flag_start = false;
    unsigned long start_millis, current_millis;
    PubSubClient* client_ptr = NULL;
    Smartclamp_AS7341* sensor_ptr = NULL;

    // Wifi and MQTT Broker Authentication
    const char* ssid = "NameOfNetwork";                     // Raspberry Pi network SSID
    const char* wifi_password = "AardvarkBadgerHedgehog";   // Raspberry Pi network password
    const char* mqtt_server = "192.168.4.1";  // IP of the MQTT broker
    const char* mqtt_username = "smart"; // MQTT username
    const char* mqtt_password = "clamp"; // MQTT password

    // MQTT Topic Names
    const char* topic_login = "lab/control/login";
    const char* topic_login_response = "lab/control/loginResponse";
    const char* topic_experiment_start = "lab/control/experimentStart";
    const char* topic_experiment_stop = "lab/control/experimentStop";
    const char* topic_experiment_data = "lab/data";

    Smartclamp_Communication();
    ~Smartclamp_Communication();

    ///////////////////   CALLBACKS    ///////////////
    void callbackLoginResponse(byte* payload, unsigned int length);
    void callbackExperimentStart(byte* payload, unsigned int length, Smartclamp_AS7341* sensor_ptr);
    void callbackExperimentStop(byte* payload, unsigned int length);
    void callbackDefault(char* topic, byte* payload, unsigned int length);
    void callback(char* topic, uint8_t* payload, unsigned int length);

    ///////////////////   HANDSHAKING    ///////////////

    void connectWifi();
    void connectMQTT();
    void identifyHandshake();

    ///////////////////   SETTERS AND GETTERS    ///////////////
    void setIdentifier(int identifier);
    bool setClientPtr(PubSubClient* client_ptr);
    bool setSensorPtr(Smartclamp_AS7341* sensor_ptr);
    bool getFlagHandshake();
    bool getFlagIdentification();
    bool getFlagStart();
    int getIdentifier();
    const char* getTopicExperimentData();
    const char* getMqttServer();

    void clientLoop();

    bool publishData(const char* buffer, unsigned int n);

};

#endif