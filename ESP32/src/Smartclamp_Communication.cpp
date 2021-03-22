/*!
 *  @file Smartclamp_Smartclamp_Communication::cpp
 *
 *  This file contains all functions related to MQTT communication
 *
 *  Developed by Ahmad Nasralla (an2485@nyu.edu) 
 * 
 */

#include <Smartclamp_Communication.h>

/**
 * @brief Construct a new Smartclamp_Communication::Smartclamp_Communication object
 *
 */
Smartclamp_Communication::Smartclamp_Communication(){
    PubSubClient client(mqtt_server, 1883, wifiClient);
    client_ptr = &client;
};

/**
 * @brief Destroy the Smartclamp_Communication::Smartclamp_Communication object
 *
 */
Smartclamp_Communication::~Smartclamp_Communication(){}


void Smartclamp_Communication::callback_login_response(char* topic, byte* payload, unsigned int length)
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
        if (client_ptr->unsubscribe(topic_login_response))
        {
            Serial.println("Unsubscribed from loginResponse topic!");
        }
        else
        {
            Serial.println("Failed to unsubscribe from loginResponse topic.");
        }

        // Sub to experimentToggle topic
        if (client_ptr->subscribe(topic_experiment_start))
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

void Smartclamp_Communication::callback_experiment_start(char* topic, byte* payload, unsigned int length)
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
            if (client_ptr->unsubscribe(topic_experiment_start))
            {
                Serial.println("Unsubscribed from experimentStart topic!");
            }
            else
            {
                Serial.println("Failed to unsubscribe from experimentStart topic.");
            }

            // Sub to experiment stop topic
            if (client_ptr->subscribe(topic_experiment_stop))
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

void Smartclamp_Communication::callback_experiment_stop(char* topic, byte* payload, unsigned int length)
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
            if (client_ptr->unsubscribe(topic_experiment_stop))
            {
                Serial.println("Unsubscribed from experimentStop topic!");
            }
            else
            {
                Serial.println("Failed to unsubscribe from experimentStop topic.");
            }

            // Sub to experiment start topic
            if (client_ptr->subscribe(topic_experiment_start))
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

void Smartclamp_Communication::callback_default(char* topic, byte* payload, unsigned int length)
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
void Smartclamp_Communication::callback(char* topic, uint8_t* payload, unsigned int length)
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

// Custom function to connect to WiFi
void Smartclamp_Communication::connect_wifi()
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
void Smartclamp_Communication::connect_MQTT()
{
    // Connect to MQTT Broker
    // client.connect returns a boolean value to let us know if the connection was successful.
    // If the connection is failing, make sure you are using the correct MQTT Username and Password
    if (client_ptr->connect(clientID, mqtt_username, mqtt_password))
    {
        Serial.println("Connected to MQTT Broker!");
        // std::function <void (char *topic, uint8_t *payload, unsigned int length)> fn1;
        // std::bind(callback);
        using std::placeholders::_1;
        using std::placeholders::_2;
        using std::placeholders::_3;
        client_ptr->setCallback(std::bind(&Smartclamp_Communication::callback, this, _1, _2, _3));
    }
    else
    {
        Serial.println("Connection to MQTT Broker failed...");
    }
}


// Custom function to obtain ID from Raspberry Pi
void Smartclamp_Communication::identify_handshake()
{

    StaticJsonDocument<256> doc;
    doc["MAC"] = macAddress.c_str();

    char buffer[256];
    size_t n = serializeJson(doc, buffer);  
    
    if (client_ptr->subscribe(topic_login_response))
    {
        Serial.println("Subscribed to login response topic!");
    }
    else
    {
        Serial.println("Failed to subscribe to login response topic.");
    }

    if (client_ptr->publish(topic_login, buffer, n))
    {
        Serial.println("MAC sent!");
    }
    else
    {
        Serial.println("String failed to send. Reconnecting to MQTT Broker and trying again");
        connect_MQTT();
    }
}

void Smartclamp_Communication::setIdentifier(int identifier)
{
    this->identifier = identifier;
}

void Smartclamp_Communication::clientLoop()
{
    client_ptr->loop();
}

bool Smartclamp_Communication::getFlagHandshake(){return flag_handshake;}

bool Smartclamp_Communication::getFlagIdentification(){return flag_identification;}

bool Smartclamp_Communication::getFlagStart(){return flag_start;}

int Smartclamp_Communication::getIdentifier(){return identifier;}

const char* Smartclamp_Communication::getTopicExperimentData(){return topic_experiment_data;}

bool Smartclamp_Communication::publishData(const char* buffer, unsigned int n)
{
    return client_ptr->publish(topic_experiment_data, buffer, n);
}