/*******************************************************************************
 * Sketch name: Digital to analog conversion
 * Description: Generate range of voltages
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    22 - ESP32 microcontroller features
 ******************************************************************************/

int DACpin = DAC1;          // define DAC pin

void setup()
{}                // nothing in setup function

void loop()
{
  for (int i=0; i<255; i=i+39)
  {
    dacWrite(DACpin, i);        // output voltage 0.5V, 1V...
    delay(2000);
  }
}
