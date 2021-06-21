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
unsigned long handshake_millis, wke_millis, slp_millis, current_millis;

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
        uint16_t readings[12];

        switch(as7341.getReadBandMode())
        {
            case AS7341_READ_ALL_CHANNELS:
                if (!as7341.readAllChannels(readings))
                {
                    if (serialDebug){
                        Serial.println("ERROR: Couldn't read all channels!");
                    }
                    return;
                }
                break;
            case AS7341_READ_LOW_CHANNELS:
                if (!as7341.readLowChannels(readings))
                {
                    if (serialDebug){
                        Serial.println("ERROR: Couldn't read low channels!");
                    }
                    return;
                }
                break;
            case AS7341_READ_HIGH_CHANNELS:
                if (!as7341.readHighChannels(readings))
                {
                    if (serialDebug){
                        Serial.println("ERROR: Couldn't read high channels!");
                    }
                    return;
                }
                break;
        }

        current_millis = millis();
        as7341.updateSensorInfo();

        StaticJsonDocument<256> doc;
        doc["ID"] = MQTT.getIdentifier();
        doc["TIME"] = current_millis;
        doc["GAIN"] = as7341.as7341Info.gain;
        doc["ATIME"] = as7341.as7341Info.atime;
        doc["ASTEP"] = as7341.as7341Info.astep;

        JsonArray data = doc.createNestedArray("DATA");

        switch(as7341.getReadBandMode())
        {
            case AS7341_READ_ALL_CHANNELS:
                for (int i = 0; i < 12; i++)
                {
                    // we skip the first set of duplicate clear/NIR readings
                    // (indices 4 and 5)
                    if (i == 4 || i == 5)
                        continue;
                    
                    data.add(readings[i]);
                }
                break;
            case AS7341_READ_LOW_CHANNELS:
                for (int i = 0; i < 6; i++)
                {
                    data.add(readings[i]);
                }
                break;
            case AS7341_READ_HIGH_CHANNELS:
                for (int i = 6; i < 12; i++)
                {
                    data.add(readings[i]);
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
}
