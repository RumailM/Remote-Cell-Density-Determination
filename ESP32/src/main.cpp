
///////////////////   LIBRARIES    ///////////////

// Communication
#include <Smartclamp_Communication.h>

// Sampling
#include <stdint.h>
#include <stdio.h>

#include <Smartclamp_LED.h>
#include <Smartclamp_Serial.h>
#include <Smartclamp_AS7341.h>

///////////////////   CONSTANTS    ///////////////

// Sampling
const unsigned long SERIAL_DELAY = 227;
const unsigned long MQTT_DELAY = 156;
const unsigned long TARGET_PERIOD = 400;
const unsigned long READING_PERIOD = TARGET_PERIOD - MQTT_DELAY;
const unsigned int AGC_FREQUENCY = 20;

///////////////////   GLOBAL VARIABLES    ///////////////

// Sampling
Smartclamp_AS7341 as7341;
unsigned long lastMsecs = millis();
bool rawCountsMode = true;
unsigned int cnt_agc = AGC_FREQUENCY+1;
unsigned long start_millis, current_millis;

// Communication
Smartclamp_Communication MQTT;
WiFiClient wifiClient;
PubSubClient client(MQTT.getMqttServer(), 1883, wifiClient);

///////////////////   SETUP    ///////////////

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(1);
    }
    Serial.println("START");
    as7341.initializeSensor();
    MQTT.setClientPtr(&client);

    MQTT.setIdentifier(-99);

    MQTT.connect_wifi();
    MQTT.connect_MQTT();

    start_millis = millis();

    // Setup LED PWM Signal.
    setupLED();
}

///////////////////   LOOP    ///////////////

int count = 0;

void loop(void)
{
    MQTT.clientLoop();

    current_millis = millis();
    if (current_millis - lastMsecs > READING_PERIOD)
    {
        Serial.print("Main Loop. Identifier: ");
        Serial.print(MQTT.getIdentifier());
        Serial.print(", flag_identification: ");
        Serial.print(MQTT.flag_identification);
        Serial.print(", flag_start: ");
        Serial.println(MQTT.getFlagStart());

        if(!MQTT.getFlagHandshake())
        {
            MQTT.identify_handshake();
        }
        else if (MQTT.getFlagIdentification() && MQTT.getFlagStart())
        {
            uint16_t readings[12];
            float counts[12];
            
            if (cnt_agc > AGC_FREQUENCY)
            {
                as7341.automaticGainContol();
                cnt_agc = 0;
            }
            cnt_agc++;
            

            if (!as7341.readAllChannels(readings))
            {
                Serial.println("ERROR: Couldn't read all channels!");
                return;
            }

            if (!rawCountsMode)
            {
                for (uint8_t i = 0; i < 12; i++)
                {
                    if (i == 4 || i == 5)
                        continue;
                    // we skip the first set of duplicate clear/NIR readings
                    // (indices 4 and 5)
                    counts[i] = as7341.toBasicCounts(readings[i]);
                }
            }

            // if (!rawCountsMode)
            //     serialAllCounts(Serial, counts);
            // else
            //     serialAllRaw(Serial, readings);

            // as7341.printParameters(Serial);

            StaticJsonDocument<256> doc;
            doc["id"] = MQTT.getIdentifier();
            doc["timestamp"] = current_millis;
            
            JsonArray data = doc.createNestedArray("readings");
            
            for (int i = 0; i < 12; i++)
            {
                data.add(readings[i]);
            }
            data.add(count++);

            JsonArray parameters = doc.createNestedArray("parameters");
            parameters.add(as7341.as7341Info.gain);
            // parameters.add(as7341.as7341Info.atime);
            // parameters.add(as7341.as7341Info.astep);
            // parameters.add(as7341.as7341Info.sp_int_en);
            // parameters.add(as7341.as7341Info.sp_agc_en);
            // parameters.add(as7341.as7341Info.agc_low_th);
            // parameters.add(as7341.as7341Info.agc_high_th);

            Serial.print("Count: ");
            Serial.println(count);

            char buffer[256];
            size_t n = serializeJson(doc, buffer);  
            
            if (MQTT.publishData(buffer, n))
            {
                Serial.println("Data sent!");
            }
            else
            {
                Serial.println("Data failed to send. Reconnecting to MQTT Broker and trying again");
                MQTT.connect_MQTT();
            }
        }
        start_millis = current_millis;
        lastMsecs = millis();
    }
}