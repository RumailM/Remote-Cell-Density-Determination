/*!
 *  @file Smartclamp_Communication.cpp
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
 * @brief Calls deserializeJson on doc and handles potential deserialization errors
 *
 * @param &doc Pointer to StaticJsonDocument of size 256 bytes 
 * @param payload Pointer to a byte array of the message
 * @param length Length of the message payload
 */
bool Smartclamp_Communication::deserializeJsonHandleError(JsonDocument& doc, byte* payload, unsigned int length)
{
    DeserializationError err = deserializeJson(doc, payload, length);

    if (err)
    {
        if(serialDebug)
            Serial.print(F("deserializeJson() failed: "));
        if(serialDebug)
            Serial.println(err.f_str());

        switch (err.code())
        {
            case DeserializationError::EmptyInput:
                // if(serialDebug)
                    Serial.println(F("Resetting handshake and identification flags..."));
                flag_handshake = false;
                flag_identification = false;
                flag_start = false;
                led_ptr->setupLED();
                break;
            case DeserializationError::Ok:
                if(serialDebug)
                    Serial.println(F("Deserialization succeeded"));
                break;
            case DeserializationError::InvalidInput:
                if(serialDebug)
                    Serial.println(F("Invalid input!"));
                break;
            case DeserializationError::NoMemory:
                if(serialDebug)
                    Serial.println(F("Not enough memory"));
                break;
            default:
                if(serialDebug) 
                    Serial.println(F("Default error"));
                break;
        }
    }
    return err;
}

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
    if(serialDebug)
        Serial.println("Login Response received.");
    
    StaticJsonDocument<256> doc;
    if (!deserializeJsonHandleError(doc, payload, length))
    {
        const char* mac = doc["MAC"];

        if (strcmp(WiFi.macAddress().c_str(), mac) == 0)
        {
            // Assign unique identifier and unsub from loginResponse topic
            identifier = doc["ID"];
            if(serialDebug)
                Serial.printf("Device ID: %d\n", identifier);

            if (client_ptr->unsubscribe(topic_login_response))
            {
                if(serialDebug)
                    Serial.println("Unsubscribed from loginResponse topic!");
            }
            else
            {
                if(serialDebug)
                    Serial.println("Failed to unsubscribe from loginResponse topic.");
            }

            // Sub to experimentToggle topic
            if (client_ptr->subscribe(topic_experiment_start))
            {
                if(serialDebug)
                    Serial.println("Subscribed to experimentStart topic!");
            }
            else
            {
                if(serialDebug)
                    Serial.println("Failed to subscribe to experimentStart topic.");
            }

            // Sub to AGCToggle topic
            if (client_ptr->subscribe(topic_AGC_toggle))
            {
                if(serialDebug)
                    Serial.println("Subscribed to AGCToggle topic!");
            }
            else
            {
                if(serialDebug)
                    Serial.println("Failed to subscribe to AGCToggle topic.");
            }

            // Raise identification flag
            flag_identification = true;
        }
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
    if(serialDebug4)
        Serial.println("callback is being triggered");
    if (flag_identification)
    {
        StaticJsonDocument<256> doc;
        if (!deserializeJsonHandleError(doc, payload, length))
        {
            if(serialDebug4)
                Serial.println("has been desearlized");
            // Check identifier
            if (identifier == (int) doc["ID"])
            {
                if(serialDebug4)
                    Serial.println("id is corrects");
                // Set ATIME and ASTEP values
                uint8_t atime = (uint8_t) doc["ATIME"];
                uint16_t astep = (uint16_t) doc["ASTEP"];
                as7341_read_band_mode readMode = static_cast<as7341_read_band_mode>(doc["MODE"]);
                lz7_color color = static_cast<lz7_color>(doc["CLR"]);
                uint16_t wakeTime = (uint16_t) doc["WKE"]; 
                uint16_t sleepTime = (uint16_t) doc["SLP"]; 

                if(serialDebug)
                    Serial.print("ATIME set to ");
                if (atime != 0)
                {
                    sensor_ptr->setATIME(atime);
                    if(serialDebug)
                        Serial.printf("custom value: %d\n", atime);
                }
                else
                {
                    sensor_ptr->setATIME(DEFAULT_ATIME);
                    if(serialDebug)
                        Serial.printf("default value: %d\n", DEFAULT_ATIME);
                }
                if(serialDebug)
                    Serial.print("ASTEP set to ");
                if (astep != 0)
                {
                    sensor_ptr->setASTEP(astep);
                    if(serialDebug)
                        Serial.printf("custom value: %d\n", astep);
                }
                else
                {
                    sensor_ptr->setASTEP(DEFAULT_ASTEP);
                    if(serialDebug)
                        Serial.printf("default value: %d\n", DEFAULT_ASTEP);
                }
                if(serialDebug)
                    Serial.print("READ_BAND_MODE set to ");
                if (readMode == AS7341_READ_LOW_CHANNELS || readMode == AS7341_READ_HIGH_CHANNELS)
                {
                    sensor_ptr->setReadBandMode(readMode);
                    if(serialDebug)
                        Serial.printf("custom value: %d\n", readMode);
                }
                else
                {
                    sensor_ptr->setReadBandMode(DEFAULT_READ_BAND_MODE);
                    if(serialDebug)
                        Serial.printf("default value: %d\n", DEFAULT_READ_BAND_MODE);
                }
                if(serialDebug)
                    Serial.print("COLOR set to ");
                if (color == LZ7_COLOR_RED || color == LZ7_COLOR_GREEN || color == LZ7_COLOR_BLUE)
                {
                    led_ptr->setColor(color);
                    if(serialDebug)
                        Serial.printf("custom value: %d\n", color);
                }
                else
                {
                    led_ptr->setColor(DEFAULT_LZ7_COLOR);
                    if(serialDebug)
                        Serial.printf("default value: %d\n", DEFAULT_LZ7_COLOR);
                }
                if(serialDebug)
                    Serial.print("WAKETIME set to ");
                if (wakeTime != 0)
                {
                    led_ptr->setWakeTime(wakeTime);
                    if(serialDebug)
                        Serial.printf("custom value: %d seconds\n", wakeTime);
                }
                else
                {
                    led_ptr->setWakeTime(DEFAULT_WAKE_TIME);
                    if(serialDebug)
                        Serial.printf("default value: %d seconds\n", DEFAULT_WAKE_TIME);
                }
                if(serialDebug)
                    Serial.print("SLEEPTIME set to ");
                if (sleepTime != 0)
                {
                    led_ptr->setSleepTime(sleepTime);
                    if(serialDebug)
                        Serial.printf("custom value: %d seconds\n", sleepTime);
                }
                else
                {
                    led_ptr->setSleepTime(DEFAULT_SLEEP_TIME);
                    if(serialDebug)
                        Serial.printf("default value: %d seconds\n", DEFAULT_SLEEP_TIME);
                }

                if (led_ptr->color != LZ7_COLOR_NONE)
                {
                    led_ptr->turnOnLight(led_ptr->getChannelFromColor(led_ptr->getColor()));
                    led_ptr->isAwake = true;
                    delay(200);
                }

                sensor_ptr->automaticGainControl();
                sensor_ptr->updateSensorInfo();
                // led_ptr->slp_millis = millis();
                // sensor_ptr->initializeReadings();

                // Unsub from experimentStart topic
                if (client_ptr->unsubscribe(topic_experiment_start))
                {
                    if(serialDebug) 
                        Serial.println("Unsubscribed from experimentStart topic!");
                }
                else
                {
                    if(serialDebug)
                        Serial.println("Failed to unsubscribe from experimentStart topic.");
                }

                // Sub to experimentStop topic
                if (client_ptr->subscribe(topic_experiment_stop))
                {
                    if(serialDebug)
                        Serial.println("Subscribed to experimentStop topic!");
                }
                else
                {
                    if(serialDebug)
                        Serial.println("Failed to subscribe to experimentStop topic.");
                }

                // Raise flag_start
                flag_start = true;
                
                if(serialDebug4)
                    Serial.println("does this get called?");
                sendExperimentStartACK();
            }

            if(serialDebug)
                Serial.println("Experiment successfully started!");
        }
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
        if (!deserializeJsonHandleError(doc, payload, length))
        {
            // Check identifier
            if (identifier == (int) doc["ID"])
            {
                // Unsub from experimentStop topic
                if (client_ptr->unsubscribe(topic_experiment_stop))
                {
                    if(serialDebug)
                        Serial.println("Unsubscribed from experimentStop topic!");
                }
                else
                {
                    if(serialDebug)
                        Serial.println("Failed to unsubscribe from experimentStop topic.");
                }
                if(!isSendingData)
                {
                    if (led_ptr->color != LZ7_COLOR_NONE)
                    {
                        led_ptr->turnOffLight(led_ptr->getChannelFromColor(led_ptr->getColor()));
                    }
                    led_ptr->isAwake = true;

                    // Temporary copy paste inclusion

                    for (int j = 0; j < sensor_ptr->subSampleIndex; ++j)
                    {
                        StaticJsonDocument<256> doc;
                        doc["ID"] = getIdentifier();
                        doc["TIME"] = sensor_ptr->times[j];
                        doc["GAIN"] = sensor_ptr->gains[j];
                        doc["ATIME"] = sensor_ptr->atimes[j];
                        doc["ASTEP"] = sensor_ptr->asteps[j];

                        JsonArray data = doc.createNestedArray("DATA");

                        switch(sensor_ptr->getReadBandMode())
                        {
                            case AS7341_READ_ALL_CHANNELS:
                                for (int i = 0; i < 12; ++i)
                                {
                                    // we skip the first set of duplicate clear/NIR readings
                                    // (indices 4 and 5)
                                    if (i == 4 || i == 5)
                                        continue;
                                    
                                    data.add(sensor_ptr->subSamples[j][i]);
                                }
                                break;
                            case AS7341_READ_LOW_CHANNELS:
                                for (int i = 0; i < 6; ++i)
                                {
                                    data.add(sensor_ptr->subSamples[j][i]);
                                }
                                break;
                            case AS7341_READ_HIGH_CHANNELS:
                                for (int i = 6; i < 12; ++i)
                                {
                                    data.add(sensor_ptr->subSamples[j][i]);
                                }
                                break;
                        }
                        char buffer[256];
                        size_t n = serializeJson(doc, buffer); 
                        
                        if (!publishData(buffer, n))
                        {
                            connectMQTT();
                        }
                    }
                }
                // sensor_ptr->initializeReadings();

                // Sub to experimentStart topic
                if (client_ptr->subscribe(topic_experiment_start))
                {
                    if(serialDebug)
                        Serial.println("Subscribed to experimentStart topic!");
                }
                else
                {
                    if(serialDebug)
                        Serial.println("Failed to subscribe to experimentStart topic.");
                }

                // Lower flag_start
                flag_start = false;
                sendExperimentStopACK();
            }

            if(serialDebug)
                Serial.println("Experiment successfully stopped!");
        }
    }
}

/**
 * @brief Callback function executed when a message is received on AGCToggle topic
 *        Runs the Automatic Gain Control (AGC) on the AS7341 sensor
 *
 * @param payload Pointer to a byte array of the message
 * @param length Length of the message payload
 */
void Smartclamp_Communication::callbackAGCToggle(byte* payload, unsigned int length)
{
    if (flag_identification)
    {
        StaticJsonDocument<256> doc;
        if (!deserializeJsonHandleError(doc, payload, length))
        {
            // Check identifier
            if (identifier == (int) doc["ID"])
            {
                sensor_ptr->automaticGainControl();
            }
            if(serialDebug)
                Serial.println("Performed Automatic Gain Calibration (AGC)!");
        }
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
    deserializeJsonHandleError(doc, payload, length);

    if(serialDebug)
        Serial.println("WARNING: Message arrived on unexpected topic");
    if(serialDebug)
        Serial.println(topic);
    if(serialDebug)
        Serial.println("Message payload:");
    for (int i = 0; i < length; i++)
    {
        if(serialDebug)
            Serial.print((char)payload[i]);
    }
    if(serialDebug)
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
        callbackAGCToggle(payload, length);
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
    if(serialDebug)
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
    if(serialDebug)
        Serial.println("WiFi connected");
    if(serialDebug)
        Serial.printf("IP address: %d.%d.%d.%d\n", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
    if(serialDebug)
        Serial.printf("MAC address: %s\n", WiFi.macAddress().c_str());
}

/**
 * @brief Custom function to connect to the MQTT broker via WiFi
 *        Connects to MQTT broker and assigns disambiguation callback function
 */
void Smartclamp_Communication::connectMQTT()
{
    // Connect to MQTT Broker
    if(serialDebug)
        Serial.println("Attempting to connect to MQTT server...");

    if (client_ptr->connect(clientID, mqtt_username, mqtt_password))
    {
        if(serialDebug)
            Serial.println("Connected to MQTT Broker!");
        using std::placeholders::_1;
        using std::placeholders::_2;
        using std::placeholders::_3;
        client_ptr->setCallback(std::bind(&Smartclamp_Communication::callback, this, _1, _2, _3));
    }
    else
    {
        if(serialDebug)
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
        if(serialDebug)
            Serial.println("Subscribed to login response topic!");
    }
    else
    {
        if(serialDebug)
            Serial.println("Failed to subscribe to login response topic.");
    }

    if (client_ptr->publish(topic_login, buffer, n))
    {
        if(serialDebug)
            Serial.println("MAC sent!");
    }
    else
    {
        if(serialDebug)
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

bool Smartclamp_Communication::setSensorPtr(Smartclamp_AS7341* sensor_ptr)
{
    this->sensor_ptr = sensor_ptr;

    return sensor_ptr != NULL;
}

bool Smartclamp_Communication::setLEDPtr(Smartclamp_LED* led_ptr)
{
    this->led_ptr = led_ptr;

    return led_ptr != NULL;
}

// Getters
bool Smartclamp_Communication::getFlagHandshake(){return flag_handshake;}

bool Smartclamp_Communication::getFlagIdentification(){return flag_identification;}

bool Smartclamp_Communication::getFlagStart(){return flag_start;}

int Smartclamp_Communication::getIdentifier(){return identifier;}

const char* Smartclamp_Communication::getTopicExperimentData(){return topic_experiment_data;}

const char* Smartclamp_Communication::getMqttServer(){return mqtt_server;}

// ACKs

void Smartclamp_Communication::sendExperimentStartACK()
{
    StaticJsonDocument<256> doc;
    doc["ID"] = getIdentifier();
    char buffer[256];
    size_t n = serializeJson(doc, buffer);
    if (client_ptr->publish(topic_experiment_start_ack, buffer, n))
    {
        if(serialDebug4)
            Serial.println("ExperimentStart ACK sent!");
    }


}
void Smartclamp_Communication::sendExperimentStopACK()
{
    StaticJsonDocument<256> doc;
    doc["ID"] = getIdentifier();
    char buffer[256];
    size_t n = serializeJson(doc, buffer);
    if (client_ptr->publish(topic_experiment_stop_ack, buffer, n))
    {
        if(serialDebug4)
            Serial.println("ExperimentStop ACK sent!");
    }

}