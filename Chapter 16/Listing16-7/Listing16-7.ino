/*******************************************************************************
 * Sketch name: Storing sine wave values in flash memory
 * Description: generate sine waves
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    16 - Signal generation
 ******************************************************************************/

#include <Adafruit_MCP4725.h>        // include Adafruit MCP4725 lib
Adafruit_MCP4725 dac;         // associate dac and MCP4725 lib
const uint16_t lookup[] PROGMEM = {     // sine wave in flash memory
2047,2402,2747,3071,3363,3615,3820,3971,4063,4094,
4063,3971,3820,3615,3363,3071,2747,2402,2047,1692,
1347,1024, 731, 479, 274, 123,  31,   0,  31, 123,
 274, 479, 731,1024,1347,1692       //  2047(sin(x/180) + 1)
};
int value, cycle;
unsigned long lastTime = 0;
float freq;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  dac.begin(0x60);          // I2C address of MCP4725
}

void loop()
{
  lastTime = millis();          // start timer
  while (cycle < 500)         // sine wave for 500 cycles
  {
    for (int i=0; i<36; i++)
    {
      value = pgm_read_word(lookup+i);    // values from flash memory
      dac.setVoltage(value, false);     // set output voltage
    }
    cycle++;
    yield();            // required to prevent timeout
  }
  freq = 1000 * 500.0/(millis() - lastTime);    // wave frequency
  Serial.print("lookup ");Serial.print(freq);
  cycle = 0;
  
  lastTime = millis();
  while (cycle < 500)
  {
    for (int deg=0; deg<360; deg=deg+10)      // generate sine wave
    {
      value = 2047.0*(sin(deg*PI/180.0)+1);   // scaled sine wave value
      dac.setVoltage(value, false);       // set output voltage
    }
    cycle++;
    yield();
  }
  freq = 1000 * 500.0/(millis()-lastTime);
  Serial.print("\tcalc   ");Serial.println(freq);
  cycle = 0;
}
