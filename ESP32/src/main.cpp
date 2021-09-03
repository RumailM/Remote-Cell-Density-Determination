///////////////////   LIBRARIES    ///////////////

// Sampling
#include <stdint.h>
#include <stdio.h>

#include <Smartclamp_LED.h>
#include <Smartclamp_Serial.h>
#include <Smartclamp_AS7341.h>
#include <Smartclamp_Communication.h>

///////////////////   CONSTANTS    ///////////////

const bool serialDebug = false;
const bool rawCountsMode = true;
const bool serialDebug2 = false;
const bool serialDebug3 = false;

///////////////////   GLOBAL     ///////////////

// Objects
Smartclamp_AS7341 as7341;
Smartclamp_Communication MQTT;
Smartclamp_LED lz7;
WiFiClient wifiClient;
PubSubClient client(MQTT.getMqttServer(), 1883, wifiClient);

// Variables
unsigned long handshake_millis, current_millis;
bool startCalledBack = false;
int globalSubSampleIndex;


///////////////////   SETUP    ///////////////

void setup()
{
    // Initialize Serial connection for debugging
    Serial.begin(115200);
    while (!Serial){delay(1);}
    Serial.println("START");

    // Initialize AS7341 Optical Sensor
    if (!as7341.initializeSensor())
    {
        if(serialDebug)
            Serial.println("ERROR: Failed to communicate with AS7341");
        delay(10000);
    }

    // // Intialize BQ27441 Fuel Gauge
    // if (lipo.begin())
    //     lipo.setCapacity(2000);
    // else
    //     Serial.println("ERROR: Failed to communicate with BQ27441");

    // Intitialize LZ7 LED Array
    lz7.setupLED();

    // Intitialize MQTT Communication
    if (!MQTT.setClientPtr(&client))
        if(serialDebug)
            Serial.println("ERROR: MQTT client_ptr is null!");
    if (!MQTT.setSensorPtr(&as7341))
        if(serialDebug)
            Serial.println("ERROR: MQTT sensor_ptr is null!");
    if (!MQTT.setLEDPtr(&lz7))
        if(serialDebug)
            Serial.println("ERROR: MQTT led_ptr is null!");
    MQTT.setIdentifier(-99);
    MQTT.connectWifi();
    MQTT.connectMQTT();

    handshake_millis = millis();
    as7341.subSampleIndex = 0;
}

///////////////////   LOOP    ///////////////

void loop(void)
{   //Serial.println("spinning");
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
    else if(startCalledBack == true && MQTT.getFlagStart() == false) 
    { // If recently stopped experiment, reset local flag
        startCalledBack = false;
        if(serialDebug3){
            Serial.println("Reset startcalledbackf flag");
            Serial.println("Sleep Millis : " + String(lz7.slp_millis) + " Current Millis: " + String(current_millis));
        }
    }
    else if (MQTT.getFlagIdentification() && MQTT.getFlagStart())
    {
        Serial.println(">>flag started");
        Serial.println("important flag is  :" + String(MQTT.getFlagStart()));
        if (startCalledBack == false )
        {
                startCalledBack = true;
                lz7.slp_millis = current_millis;
                if(serialDebug3){
                    Serial.println("Updated sleep millis");
                    Serial.println("Sleep Millis : " + String(lz7.slp_millis) + " Current Millis: " + String(current_millis));

                }
        }

        if (lz7.isAwake)
        {               
            Serial.println("line 116 lz7.isAwake");
            Serial.println("Sleep Millis : " + String(lz7.slp_millis) + " Current Millis: " + String(current_millis));
            Serial.println("lz7.wakeTime is :" + String(lz7.wakeTime));
            if (current_millis - lz7.slp_millis > lz7.wakeTime * 1000)
            {
                lz7.isAwake = false;
                if (lz7.color != LZ7_COLOR_NONE)
                {
                    lz7.turnOffLight(lz7.getChannelFromColor(lz7.getColor()));
                }
                lz7.slp_millis = current_millis;

                for (int j = 0; j < as7341.subSampleIndex; ++j)
                {
                    if(serialDebug3)
                        Serial.println("going through subsamples");
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
                            for (int i = 0; i < 12; ++i)
                            {
                                // we skip the first set of duplicate clear/NIR readings
                                // (indices 4 and 5)
                                if (i == 4 || i == 5)
                                    continue;
                                
                                data.add(as7341.subSamples[j][i]);
                            }
                            break;
                        case AS7341_READ_LOW_CHANNELS:
                            for (int i = 0; i < 6; ++i)
                            {
                                data.add(as7341.subSamples[j][i]);
                            }
                            break;
                        case AS7341_READ_HIGH_CHANNELS:
                            for (int i = 6; i < 12; ++i)
                            {
                                data.add(as7341.subSamples[j][i]);
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
                as7341.subSampleIndex = 0;

                // as7341.initializeReadings();
            }
            else
            {
                switch(as7341.getReadBandMode())
                {
                    case AS7341_READ_ALL_CHANNELS:
                        if (!as7341.readAllChannels(as7341.subSamples[as7341.subSampleIndex]))
                        {
                            if (serialDebug){
                                Serial.println("ERROR: Couldn't read all channels!");
                            }
                            return;
                        }
                        break;
                    case AS7341_READ_LOW_CHANNELS:
                        if (!as7341.readLowChannels(as7341.subSamples[as7341.subSampleIndex]))
                        {
                            if (serialDebug){
                                Serial.println("ERROR: Couldn't read low channels!");
                            }
                            return;
                        }
                        break;
                    case AS7341_READ_HIGH_CHANNELS:
                        if (!as7341.readHighChannels(as7341.subSamples[as7341.subSampleIndex]))
                        {
                            if (serialDebug){
                                Serial.println("ERROR: Couldn't read high channels!");
                            }
                            return;
                        }
                        break;
                }

                // as7341.updateSensorInfo();
                // as7341.readingsPointer += 12;
                // as7341.times.push_back(current_millis);
                // as7341.gains.push_back(as7341.as7341Info.gain);
                // as7341.atimes.push_back(as7341.as7341Info.atime);
                // as7341.asteps.push_back(as7341.as7341Info.astep);
                as7341.times[as7341.subSampleIndex] = current_millis;
                as7341.gains[as7341.subSampleIndex] = as7341.as7341Info.gain;
                as7341.atimes[as7341.subSampleIndex] = as7341.as7341Info.atime;
                as7341.asteps[as7341.subSampleIndex] = as7341.as7341Info.atime;
                as7341.subSampleIndex++;
                if(serialDebug2)
                    Serial.println("just read into subsample infdex : " +String(as7341.subSampleIndex));
            
            }
        }
        else
        {
            Serial.println("line 236 lz7.isAwake not awake");
            Serial.println("Sleep Millis : " + String(lz7.slp_millis) + " Current Millis: " + String(current_millis));
            Serial.println("lz7.sleepTime is :" + String(lz7.sleepTime));

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
                if(serialDebug2)
                    Serial.println("slp_millis is : "+String(lz7.slp_millis));
                if(serialDebug2)
                    Serial.println("current millis is : "+String(current_millis));
                 delay(500);
            }
        }
    } else {
        Serial.println("For some reason some weird stuff is happenign and we are here now");
        Serial.println("Sleep Millis : " + String(lz7.slp_millis) + " Current Millis: " + String(current_millis));
        Serial.println("important flag is  :" + String(MQTT.getFlagStart()));
        delay(2000);
    }
}
