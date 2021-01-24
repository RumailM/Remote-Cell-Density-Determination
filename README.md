# Remote-Cell-Density-Determination

Ongoing developing a low-cost cell density monitoring accessory for standard laboratory glassware with NetBio Research Group at New York University Abu Dhabi (NYUAD). The project's primary mission is to reduce the manual labor burden of growing large batches of cells such that lab technicians may focus their time on more personally and professionally fulfilling work.

## Hardware Used

- ESP-WROVER-KIT
- ADAFRUIT AS7341
- 56 Ohm Resistor
- ThorLabs LED631E

## ESP-WROVER-KIT Hardware Connections

- Power: microUSB port
- VIN: 3.3V
- I2C SDA: IO21
- I2C SCL: IO22
- LED PWM:

## Compilation Instructions

This code is designed to be deployed using PlatformIO on either VSCode or Atom. Before attempting to build or upload the code, make sure to verify the upload_port parameter is correct on the platform.ini file.