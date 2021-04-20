/*!
 *  @file Smartclamp_Smartclamp_Communication::cpp
 *
 *  This file contains all functions related to MQTT communication
 * 
 *  It utilizes the PubSubClient Arduino MQTT library available at:
 *  https://github.com/knolleary/pubsubclient
 *
 *  Developed by Ahmad Nasralla (an2485@nyu.edu) 
 * 
 */

#include <Smartclamp_Communication.h>

/**
 * @brief Construct a new Smartclamp_Communication::Smartclamp_Communication object
 * 
 */
Smartclamp_Communication::Smartclamp_Communication(){}

/**
 * @brief Destroy the Smartclamp_Communication::Smartclamp_Communication object
 * 
 */
Smartclamp_Communication::~Smartclamp_Communication(){}

/**
 * @brief Callback function executed when a message is received on loginResponse topic
 *        Assigns device identifier and subscribes to experimentStart topic
 *
 * @param payload Pointer to a byte array of the message
 * @param length Length of the message payload
 */
void Smartclamp_Communication::callbackLoginResponse(byte* payload, unsigned int length)
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

    if (strcmp(WiFi.macAddress().c_str(), mac) == 0)
    {
        // Assign unique identifier and unsub from loginResponse topic
        identifier = doc["id"];
        Serial.printf("Device ID: %d\n", identifier);

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

/**
 * @brief Callback function executed when a message is received on experimentStart topic
 *        Starts experiment on device specified by identifier
 *        If message identifier matches device identifier:
 *              Unsubs from experimentStart topic, Subs to experimentStop topic
 *              Raises flag to begin sensor reading and transmission
 *
 * @param payload Pointer to a byte array of the message
 * @param length Length of the message payload
 */
void Smartclamp_Communication::callbackExperimentStart(byte* payload, unsigned int length, Smartclamp_AS7341* sensor_ptr)
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
        if (identifier == (int) doc["id"])
        {
            // Set ATIME and ASTEP values
            uint8_t atime = (uint8_t) doc["ATIME"];
            uint16_t astep = (uint16_t) doc["ASTEP"];
            unsigned int agc = (unsigned int) doc["AGC"];
            as7341_read_band_mode readMode = static_cast<as7341_read_band_mode>(doc["MODE"]);

            Serial.print("ATIME set to ");
            if (atime != 0)
            {
                sensor_ptr->setATIME(atime);
                Serial.printf("custom value: %d\n", atime);
            }
            else
            {
                sensor_ptr->setATIME(DEFAULT_ATIME);
                Serial.printf("default value: %d\n", DEFAULT_ATIME);
            }
            Serial.print("ASTEP set to ");
            if (astep != 0)
            {
                sensor_ptr->setASTEP(astep);
                Serial.printf("custom value: %d\n", astep);
            }
            else
            {
                sensor_ptr->setASTEP(DEFAULT_ASTEP);
                Serial.printf("default value: %d\n", DEFAULT_ASTEP);
            }
            Serial.print("AGC_FREQUENCY set to ");
            if (agc != 0)
            {
                sensor_ptr->setAgcFrequency(agc);
                Serial.printf("custom value: %d\n", agc);
            }
            else
            {
                sensor_ptr->setAgcFrequency(DEFAULT_AGC_FREQUENCY);
                Serial.printf("default value: %d\n", DEFAULT_AGC_FREQUENCY);
            }
            Serial.print("READ_BAND_MODE set to ");
            if (readMode == AS7341_READ_LOW_CHANNELS || readMode == AS7341_READ_HIGH_CHANNELS)
            {
                sensor_ptr->setReadBandMode(readMode);
                Serial.printf("custom value: %d\n", readMode);
            }
            else
            {
                sensor_ptr->setReadBandMode(DEFAULT_READ_BAND_MODE);
                Serial.printf("default value: %d\n", DEFAULT_READ_BAND_MODE);
            }
            
            // Unsub from experimentStart topic
            if (client_ptr->unsubscribe(topic_experiment_start))
            {
                Serial.println("Unsubscribed from experimentStart topic!");
            }
            else
            {
                Serial.println("Failed to unsubscribe from experimentStart topic.");
            }

            // Sub to experimentStop topic
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

/**
 * @brief Callback function executed when a message is received on experimentStop topic
 *        Stops experiment on device specified by identifier
 *        If message identifier matches device identifier:
 *              Unsubs from experimentStop topic, Subs to experimentStart topic
 *              Lowers flag to stop sensor reading and transmission
 *
 * @param payload Pointer to a byte array of the message
 * @param length Length of the message payload
 */
void Smartclamp_Communication::callbackExperimentStop(byte* payload, unsigned int length)
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
            // Unsub from experimentStop topic
            if (client_ptr->unsubscribe(topic_experiment_stop))
            {
                Serial.println("Unsubscribed from experimentStop topic!");
            }
            else
            {
                Serial.println("Failed to unsubscribe from experimentStop topic.");
            }

            // Sub to experimentStart topic
            if (client_ptr->subscribe(topic_experiment_start))
            {
                Serial.println("Subscribed to experimentStart topic!");
            }
            else
            {
                Serial.println("Failed to subscribe to experimentStart topic.");
            }

            // Lower flag_start
            flag_start = false;
        }

        Serial.println("Experiment successfully stopped!");
    }
}

/**
 * @brief Callback function executed when a message is received on AGCToggle topic
 *        Runs the Automatic Gain Control (AGC) on the AS7341 sensor
 *
 * @param payload Pointer to a byte array of the message
 * @param length Length of the message payload
 */
void Smartclamp_Communication::callbackAGCToggle(byte* payload, unsigned int length, Smartclamp_AS7341* sensor_ptr)
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
        if (identifier == (int) doc["id"])
        {
            sensor_ptr->automaticGainContol();
        }
        Serial.println("Performed Automatic Gain Calibration (AGC)!");
    }
}

/**
 * @brief Callback function executed when a message is received on unexpected topic
 *        Prints warning of unexpected topic name and message contents
 *
 * @param topic Pointer to a character array of the message topic name
 * @param payload Pointer to a byte array of the message
 * @param length Length of the message payload
 */
void Smartclamp_Communication::callbackDefault(char* topic, byte* payload, unsigned int length)
{
    StaticJsonDocument<256> doc;
    DeserializationError err = deserializeJson(doc, payload, length);

    if (err)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(err.f_str());
        return;
    }

    Serial.println("WARNING: Message arrived on unexpected topic");
    Serial.println(topic);
    Serial.println("Message payload:");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

/**
 * @brief Callback function disambiguation
 *        Calls the appropriate callback function based upon received message topic
 *
 * @param topic Pointer to a character array of the message topic name
 * @param payload Pointer to a byte array of the message
 * @param length Length of the message payload
 */
void Smartclamp_Communication::callback(char* topic, uint8_t* payload, unsigned int length)
{
    if (strcmp(topic, topic_login_response) == 0)
    {
        callbackLoginResponse(payload, length);
    }
    else if (strcmp(topic, topic_experiment_start) == 0)
    {
        callbackExperimentStart(payload, length, sensor_ptr);
    }
    else if (strcmp(topic, topic_experiment_stop) == 0)
    {
        callbackExperimentStop(payload, length);
    }
    else if (strcmp(topic, topic_AGC_toggle) == 0)
    {
        callbackAGCToggle(payload, length, sensor_ptr);
    }
    else
    {
        callbackDefault(topic, payload, length);
    }
}

/**
 * @brief Custom function to connect to WiFi
 * 
 */
void Smartclamp_Communication::connectWifi()
{
    Serial.printf("Connecting to %s\n", ssid);

    // Connect to the WiFi
    WiFi.begin(ssid, wifi_password);

    // Wait until the connection has been confirmed before continuing
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    // Debugging - Output the IP Address and MAC Address
    Serial.println("WiFi connected");
    Serial.printf("IP address: %d.%d.%d.%d\n", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
    Serial.printf("MAC address: %s\n", WiFi.macAddress().c_str());
}

/**
 * @brief Custom function to connect to the MQTT broker via WiFi
 *        Connects to MQTT broker and assigns disambiguation callback function
 */
void Smartclamp_Communication::connectMQTT()
{
    // Connect to MQTT Broker
    Serial.println("Attempting to connect to MQTT server...");

    if (client_ptr->connect(clientID, mqtt_username, mqtt_password))
    {
        Serial.println("Connected to MQTT Broker!");
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

/**
 * @brief Custom function to obtain assigned identifier from Raspberry Pi
 *        Subs to loginResponse topic, Publishes MAC address to login topic for server identification
 *        Reconnects to MQTT broker if messages fails to send
 */
void Smartclamp_Communication::identifyHandshake()
{
    StaticJsonDocument<256> doc;
    doc["MAC"] = WiFi.macAddress().c_str();

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
        connectMQTT();
    }
}

/**
 * @brief Publishes experiment data to the data topic
 * 
 */
bool Smartclamp_Communication::publishData(const char* buffer, unsigned int n)
{
    return client_ptr->publish(topic_experiment_data, buffer, n);
}

/**
 * @brief Allows the client to process incoming messages and maintain its connection to the server
 * 
 */
void Smartclamp_Communication::clientLoop()
{
    client_ptr->loop();
}

// Setters
void Smartclamp_Communication::setIdentifier(int identifier)
{
    this->identifier = identifier;
}

bool Smartclamp_Communication::setClientPtr(PubSubClient* client_ptr)
{
    this->client_ptr = client_ptr;

    return client_ptr != NULL;
}

bool Smartclamp_Communication::setSensorPtr(Smartclamp_AS7341* sesnsor_ptr)
{
    this->sensor_ptr = sesnsor_ptr;

    return sensor_ptr != NULL;
}

// Getters
bool Smartclamp_Communication::getFlagHandshake(){return flag_handshake;}

bool Smartclamp_Communication::getFlagIdentification(){return flag_identification;}

bool Smartclamp_Communication::getFlagStart(){return flag_start;}

int Smartclamp_Communication::getIdentifier(){return identifier;}

const char* Smartclamp_Communication::getTopicExperimentData(){return topic_experiment_data;}

const char* Smartclamp_Communication::getMqttServer(){return mqtt_server;}