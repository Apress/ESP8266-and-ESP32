/*******************************************************************************
 * Sketch name: Generating a sine wave with ESP32 DAC
 * Description: generate sine waves
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    16 - Signal generation
 ******************************************************************************/

int DACpin = DAC1;          // DAC pin on GPIO 25
float angle;
int val;

void setup()            // nothing in setup function
{}

void loop()
{
  for (int deg=0; deg<360; deg++)     // cycle through 360°
  {
    angle = deg*PI/180.0;       // convert degrees to radians
    val = round(128+100.0*sin(angle));    // scaled sine wave value
    dacWrite(DACpin, val);        // output voltage
  }
}
