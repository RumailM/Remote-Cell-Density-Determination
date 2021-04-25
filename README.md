# Remote-Cell-Density-Determination

Ongoing developing a low-cost cell density monitoring accessory for standard laboratory glassware with NetBio Research Group at New York University Abu Dhabi (NYUAD). The project's primary mission is to reduce the manual labor burden of growing large batches of cells such that lab technicians may focus their time on more personally and professionally fulfilling work.

### Hardware Used

- ESP32 Sparkfun Thing Plus
- ADAFRUIT AS7341
- 56 Ohm Resistor
- ThorLabs LED631E
- 1000 mAh 3.7V LiPo Battery

### ESP32 Hardware Connections

- VIN: 3.3V
- I2C SDA: IO21
- I2C SCL: IO22
- LED PWM: IO4

### Compilation Instructions

This code is designed to be deployed using PlatformIO on either VSCode or Atom. Before attempting to build or upload the code, make sure to verify the upload_port parameter is correct on the platform.ini file.

## Raspberry Pi Hub Setup
1. Follow instructions [here](https://diyi0t.com/microcontroller-to-raspberry-pi-wifi-mqtt-communication/) under the "Setup the MQTT Broker to Receive MQTT Data (Raspberry Pi)" section.
2. Make sure the mosquitto (The MQTT Broker) is running
3. cd into the repository
4. cd into Webserver
5. source env/bin/activate
6. python3 app.py
7. Run the app.py flask server
