/*
BME280 Spi Test.ino

This code shows how to record data from the BME280 environmental sensor
using Spi interface. This file is an example file, part of the Arduino
BME280 library.

GNU General Public License

Written: Dec 30 2015.
Last Updated: Oct 07 2017.

Connecting the BME280 Sensor:
Sensor              ->  Board
-----------------------------
Vin (Voltage In)    ->  3.3V
Gnd (Ground)        ->  Gnd
SDA (Serial Data)   ->  A4 on Uno/Pro-Mini, 20 on Mega2560/Due, 2 Leonardo/Pro-Micro
SCK (Serial Clock)  ->  A5 on Uno/Pro-Mini, 21 on Mega2560/Due, 3 Leonardo/Pro-Micro

*/

#include <SPI.h>                         // Needed for legacy versions of Arduino.
#include <BME280.h>
#include <BME280Spi.h>

#define SERIAL_BAUD 9600
#define DEVICE_PIN 9




BME280Spi::Settings settings(
   DEVICE_PIN,  //SPI Device Pin
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::OSR_Off,
   BME280::Mode_Forced,
   BME280::StandbyTime_1000ms,
   BME280::Filter_Off,
   BME280::SpiEnable_False
);

BME280Spi bme(settings);  // Object Construction


//////////////////////////////////////////////////////////////////
void setup()
{
   Serial.begin(SERIAL_BAUD);

   while(!Serial) {} // Wait
  
   SPI.begin();

   while(!bme.begin())
   {
      Serial.println("Could not find BME280 sensor!");
      delay(1000);
   }

   // bme.chipID(); // Deprecated. See chipModel().
   switch(bme.chipModel())
   {
      case BME280::ChipModel_BME280:
        Serial.println("Found BME280 sensor! Success.");
        break;
      case BME280::ChipModel_BMP280:
        Serial.println("Found BMP280 sensor! No Humidity available.");
        break;
      default:
        Serial.println("Found UNKNOWN sensor! Error!");
   }
}

//////////////////////////////////////////////////////////////////
void loop()
{
   printBME280Data(&Serial);
   delay(1000);
}

//////////////////////////////////////////////////////////////////
void printBME280Data
(
   Stream* client
)
{
   float temp(NAN), hum(NAN), pres(NAN);

   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_hPa);

   bme.read(pres, temp, hum, tempUnit, presUnit);

   client->print("Temp: ");
   client->print(temp);
   client->print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
   client->print("\t\tHumidity: ");
   client->print(hum);
   client->print("% RH");
   client->print("\t\tPressure: ");
   client->print(pres);
   client->println(" Pa");

   delay(1000);
}
