/*******************************************************************************
 * Sketch name: I2C scanner
 * Description: Scan I2C devices to determine I2C address
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    16 - Signal generation
 ******************************************************************************/

#include <Wire.h>            // include Wire library
int device = 0;           // set device counter

void setup()
{
  Serial.begin (115200);        // define Serial Monitor baud rate
  Wire.begin();           // start I2C bus
  for (int i=8; i<127; i++)       // scan through channels 8 to 126
  {
    Wire.beginTransmission (i);     // transmit to device at address i
    if (Wire.endTransmission () == 0)   // device response to transmission
    {
      Serial.print("Address 0x");
      Serial.println(i, HEX);       // display I2C address in HEX
      device++;           // increment device count
      delay(10);
    }
  }
  Serial.print(device);         // display device count
  Serial.println(" device found");
}

void loop()             // nothing in loop function
{}
