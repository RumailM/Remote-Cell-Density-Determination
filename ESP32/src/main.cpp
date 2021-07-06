///////////////////   LIBRARIES    ///////////////

// Sampling
#include <stdint.h>
#include <stdio.h>

#include <Smartclamp_LED.h>
#include <Smartclamp_Serial.h>
#include <Smartclamp_AS7341.h>
#include <Smartclamp_Communication.h>

///////////////////   CONSTANTS    ///////////////

const extern bool serialDebug = false;
const bool rawCountsMode = true;

///////////////////   GLOBAL     ///////////////

// Objects
Smartclamp_AS7341 as7341;
Smartclamp_Communication MQTT;
Smartclamp_LED lz7;
WiFiClient wifiClient;
PubSubClient client(MQTT.getMqttServer(), 1883, wifiClient);

// Variables
unsigned long handshake_millis, current_millis;

///////////////////   SETUP    ///////////////

void setup()
{
    // Initialize Serial connection for debugging
    Serial.begin(115200);
    while (!Serial){delay(1);}
    Serial.println("START");

    // Initialize 
    as7341.initializeSensor();

    if (!MQTT.setClientPtr(&client))
        Serial.println("ERROR: client_ptr is null!");
    if (!MQTT.setSensorPtr(&as7341))
        Serial.println("ERROR: sensor_ptr is null!");
    if (!MQTT.setLEDPtr(&lz7))
        Serial.println("ERROR: led_ptr is null!");
    MQTT.setIdentifier(-99);
    MQTT.connectWifi();
    MQTT.connectMQTT();

    handshake_millis = millis();
    lz7.setupLED();
}

///////////////////   LOOP    ///////////////

void loop(void)
{
    MQTT.clientLoop();
    current_millis = millis();

    if(!MQTT.getFlagHandshake())
    {
        if (current_millis - handshake_millis > 1000)
        {
            MQTT.identifyHandshake();
            handshake_millis = current_millis;
        }
    }
    else if (MQTT.getFlagIdentification() && MQTT.getFlagStart())
    {
        if (lz7.isAwake)
        {
            if (current_millis - lz7.slp_millis > lz7.wakeTime * 1000)
            {
                lz7.isAwake = false;
                if (lz7.color != LZ7_COLOR_NONE)
                {
                    lz7.turnOffLight(lz7.getChannelFromColor(lz7.getColor()));
                }
                lz7.slp_millis = current_millis;

                for (int j = 0; j < as7341.times.size(); ++j)
                {
                    StaticJsonDocument<256> doc;
                    doc["ID"] = MQTT.getIdentifier();
                    doc["TIME"] = as7341.times[j];
                    doc["GAIN"] = as7341.gains[j];
                    doc["ATIME"] = as7341.atimes[j];
                    doc["ASTEP"] = as7341.asteps[j];

                    JsonArray data = doc.createNestedArray("DATA");

                    switch(as7341.getReadBandMode())
                    {
                        case AS7341_READ_ALL_CHANNELS:
                            for (int i = 12*j; i < 12*j+12; ++i)
                            {
                                // we skip the first set of duplicate clear/NIR readings
                                // (indices 4 and 5)
                                if (i == 12*j+4 || i == 12*j+5)
                                    continue;
                                
                                data.add(as7341.readings[i]);
                            }
                            break;
                        case AS7341_READ_LOW_CHANNELS:
                            for (int i = 12*j; i < 12*j+6; ++i)
                            {
                                data.add(as7341.readings[i]);
                            }
                            break;
                        case AS7341_READ_HIGH_CHANNELS:
                            for (int i = 12*j+6; i < 12*j+12; ++i)
                            {
                                data.add(as7341.readings[i]);
                            }
                            break;
                    }
                    char buffer[256];
                    size_t n = serializeJson(doc, buffer); 
                    if (serialDebug){Serial.println(buffer);} 
                    
                    if (MQTT.publishData(buffer, n))
                    {
                        if (serialDebug){Serial.println("Data sent!");}
                    }
                    else
                    {
                        if (serialDebug){Serial.println("Data failed to send. Reconnecting to MQTT Broker and trying again");}
                        MQTT.connectMQTT();
                    }
                }

                as7341.initializeReadings();
            }
            else
            {
                switch(as7341.getReadBandMode())
                {
                    case AS7341_READ_ALL_CHANNELS:
                        if (!as7341.readAllChannels(as7341.readingsPointer))
                        {
                            if (serialDebug){
                                Serial.println("ERROR: Couldn't read all channels!");
                            }
                            return;
                        }
                        break;
                    case AS7341_READ_LOW_CHANNELS:
                        if (!as7341.readLowChannels(as7341.readingsPointer))
                        {
                            if (serialDebug){
                                Serial.println("ERROR: Couldn't read low channels!");
                            }
                            return;
                        }
                        break;
                    case AS7341_READ_HIGH_CHANNELS:
                        if (!as7341.readHighChannels(as7341.readingsPointer))
                        {
                            if (serialDebug){
                                Serial.println("ERROR: Couldn't read high channels!");
                            }
                            return;
                        }
                        break;
                }

                as7341.updateSensorInfo();
                as7341.readingsPointer += 12;
                as7341.times.push_back(current_millis);
                as7341.gains.push_back(as7341.as7341Info.gain);
                as7341.atimes.push_back(as7341.as7341Info.atime);
                as7341.asteps.push_back(as7341.as7341Info.astep);
            }
        }
        else
        {
            if (current_millis - lz7.slp_millis > lz7.sleepTime * 1000)
            {
                lz7.isAwake = true;
                if (lz7.color != LZ7_COLOR_NONE)
                {
                    lz7.turnOnLight(lz7.getChannelFromColor(lz7.getColor()));
                }
                lz7.slp_millis = current_millis;
            }
            else
            {

            }
        }
    }
}
