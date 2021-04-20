///////////////////   LIBRARIES    ///////////////

// Sampling
#include <stdint.h>
#include <stdio.h>

#include <Smartclamp_LED.h>
#include <Smartclamp_Serial.h>
#include <Smartclamp_AS7341.h>
#include <Smartclamp_Communication.h>

///////////////////   CONSTANTS    ///////////////

// It was found that waiting between loops is unnecessary:
// read...Channels -> delayForData(0) -> getIsDataReady -> Register Ready Status
// Data will only be sent from sensor when ready, further delays on this end only decrease reading frequency

// const unsigned long SERIAL_DELAY = 227;
// const unsigned long MQTT_DELAY = 158;
// const unsigned long TARGET_PERIOD = 200;
// const unsigned long READING_PERIOD = TARGET_PERIOD - MQTT_DELAY;
// const unsigned long READING_PERIOD = (DEFAULT_ATIME+1)*(DEFAULT_ASTEP+1)*278/100000;

const extern bool serialDebug = false;
const bool rawCountsMode = true;

///////////////////   GLOBAL     ///////////////

// Objects
Smartclamp_AS7341 as7341;
Smartclamp_Communication MQTT;
WiFiClient wifiClient;
PubSubClient client(MQTT.getMqttServer(), 1883, wifiClient);

// Variables
unsigned long lastMsecs = millis();
unsigned int cnt_agc = as7341.getAgcFrequency()+1;
unsigned long current_millis;

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
    MQTT.setIdentifier(-99);
    MQTT.connectWifi();
    MQTT.connectMQTT();

    setupLED();
}

///////////////////   LOOP    ///////////////

void loop(void)
{
    MQTT.clientLoop();

    if(!MQTT.getFlagHandshake()){MQTT.identifyHandshake();}

    else if (MQTT.getFlagIdentification() && MQTT.getFlagStart())
    {
        uint16_t readings[12];
        
        if (cnt_agc > as7341.getAgcFrequency())
        {
            as7341.automaticGainContol();
            cnt_agc = 0;

            if (serialDebug){Serial.println("DEBUG: Performed AGC");}
        }
        cnt_agc++;

        if (!as7341.readHighChannels(readings))
        {
            if (serialDebug){
                Serial.println("ERROR: Couldn't read all channels!");
            }
            return;
        }
        current_millis = millis();
        as7341.updateSensorInfo();

        if (!rawCountsMode)
        {
            float counts[12];
            for (uint8_t i = 0; i < 12; i++)
            {
                if (i == 4 || i == 5)
                    continue;
                // we skip the first set of duplicate clear/NIR readings
                // (indices 4 and 5)
                counts[i] = as7341.toBasicCounts(readings[i]);
            }
            if (serialDebug){serialAllCounts(Serial, counts);}
        }
        else if (serialDebug)
        {
            serialAllRaw(Serial, readings);
            as7341.printParameters(Serial);
        }

        StaticJsonDocument<256> doc;
        doc["id"] = MQTT.getIdentifier();
        doc["timestamp"] = current_millis;
        doc["gain"] = as7341.as7341Info.gain;
        doc["atime"] = as7341.as7341Info.atime;
        doc["astep"] = as7341.as7341Info.astep;

        JsonArray data = doc.createNestedArray("readings");
        
        for (int i = 0; i < 12; i++)
        {
            data.add(readings[i]);
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