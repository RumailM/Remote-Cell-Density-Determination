
///////////////////   LIBRARIES    ///////////////

#include "Smartclamp_LED.h"
#include "Smartclamp_Serial.h"
#include "Smartclamp_AS7341.h"
#include "Smartclamp_Wireless.h"

///////////////////   CONSTANTS    ///////////////

const unsigned long SERIAL_DELAY = 227;
const unsigned long TARGET_PERIOD = 500;
const unsigned long READING_PERIOD = TARGET_PERIOD - SERIAL_DELAY;
const unsigned int AGC_FREQUENCY = 20;

///////////////////   GLOBAL VARIABLES    ///////////////

Smartclamp_AS7341 as7341;
bool rawCountsMode = true;
unsigned int cnt_agc = 0;
int count = 0;
unsigned long start_millis, current_millis;

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

    identifier = -99;

    connect_wifi();
    connect_MQTT();

    start_millis = millis();

    // Setup LED PWM Signal.
    setupLED();
}

///////////////////   LOOP    ///////////////

void loop(void)
{
    client.loop();

    current_millis = millis();
    if (current_millis - start_millis > TARGET_PERIOD)
    {
        // Serial.print("Main Loop. Identifier: ");
        // Serial.print(identifier);
        // Serial.print(", flag_identification: ");
        // Serial.print(flag_identification);
        // Serial.print(", flag_start: ");
        // Serial.println(flag_start);

        if(!flag_handshake)
        {
            identify_handshake();
        }
        else if (flag_identification && flag_start)
        {
            uint16_t readings[12];
            float counts[12];
            
            if (cnt_agc > AGC_FREQUENCY)
            {
                as7341.automaticGainContol();
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

            if (!rawCountsMode)
                serialAllCounts(Serial, counts);
            else
                serialAllRaw(Serial, readings);

            as7341.printParameters(Serial);

            StaticJsonDocument<256> doc;
            doc["id"] = identifier;
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
            
            if (client.publish(topic_experiment_data, buffer, n))
            {
                Serial.println("Data sent!");
            }
            else
            {
                Serial.println("Data failed to send. Reconnecting to MQTT Broker and trying again");
                connect_MQTT();
            }
        }
        start_millis = current_millis;
    }
    else
    {
        read_SERIAL(as7341);
    }
}