/*******************************************************************************
 * Sketch name: Sine wave with MCP4725 12-bit DAC module
 * Description: generate sine waves
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    16 - Signal generation
 ******************************************************************************/

#include <Adafruit_MCP4725.h>        // include Adafruit MCP4725 lib
Adafruit_MCP4725 dac;         // associate dac and MCP4725 lib
float voltDivid = 3200;         // voltage divider adjustment
float VCC = 3300;           // operating voltage
int flag = 0;           // flag to switch to sine wave
float predict, voltage, value;
int Vin;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  dac.begin(0x60);          // I2C address of MCP4725
}

void loop()
{
  if(flag < 1)
  {
    for(int i=0; i<4096; i=i+50)
    {
      dac.setVoltage(i, false);     // set output voltage
      Vin = analogRead(A0);       // read DAC voltage
      voltage = Vin * voltDivid /1024.0;    // scaled output voltage
      predict = i * VCC / 4096.0;     // predicted output voltage
      Serial.print(voltage);Serial.print("\t");
      Serial.println(predict);      // display voltages
    }
    delay(2000);
    flag = 1;           // switch to sine wave only
  }
  for (int deg=0; deg<360; deg++)     // generate sine wave
  {
    value = 2047.0*(sin(deg*PI/180.0)+1);   // scaled sine wave value
    dac.setVoltage(value, false);     // set output voltage
  }
}
