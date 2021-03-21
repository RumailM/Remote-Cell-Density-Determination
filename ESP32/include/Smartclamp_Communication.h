#ifndef _SMARTCLAMP_COMMUNICATION_H
#define _SMARTCLAMP_COMMUNICATION_H

#include "PubSubClient.h" // Connect and publish to the MQTT broker
#include "WiFi.h" // Enables the ESP32 to connect to the local network (via WiFi)
#include <ArduinoJson.h> // Enables JSON serialization and deserialization

class Smartclamp_Communication : public Smartclamp_Communication
{

public:
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

};

#endif