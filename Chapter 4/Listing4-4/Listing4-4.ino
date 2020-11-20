/*******************************************************************************
 * Sketch name: LED rings clock
 * Description: Clock with initial time defined in sketch
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    4 - Internet clock
 ******************************************************************************/

#include <TimeLib.h>          // include Time library
unsigned long pctime = 1590945651;      // set Unix epoch time
#include <Adafruit_NeoPixel.h>      // include NeoPixel library
int LEDpinM = D3;           // ring to display minutes
int LEDpinH = D2;           // ring to display hours
int piezoPin = D1;          // Piezo transducer pin
int LEDnumber = 12;         // number of LEDs on ring
unsigned long interval = 1000;      // one sec time interval
int color = 0;            // color flag for hour LED ring
int display = 0;            // indicator hr display completed
String text;
int minutes, hours;
// associate ringM and ringH with Neopixel library
Adafruit_NeoPixel ringM(LEDnumber, LEDpinM, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ringH(LEDnumber, LEDpinH, NEO_GRB + NEO_KHZ800);

void setup()
{
  setTime(pctime);          // set time to Unix epoch time
  getTime();            // get time parameters
  ringH.begin();            // initialise hours LED ring
  ringH.setBrightness(1);       // set LED brightness (1 to 255)
  LEDhours();           // function to display hours
  ringH.show();           // update hours LED ring
  ringM.begin();
  ringM.setBrightness(1);
  LEDminutes();
  ringM.show();           // update minutes LED ring
}

void loop()
{
  getTime();            // calculate hours and minutes
  if(minutes == 0 && display ==0)     // on the hour, run the display
  {
    display = 1;            // flag to prevent repeat hr display
    phonering();            // sound of phone ringtones
    rainbow();            // rainbow of LED colors
    ringM.clear();          // clear minutes LED ring
    ringM.show();
    color = 1 - color;          // change color flag
    ringH.clear();
    ringH.show();
    LEDhours();           // update hours LED ring
  }
  if(minutes > 0) LEDminutes();     // update minutes LED ring
  for(int i=0; i<22; i++) delay(interval);  // 22s delay
}

void getTime()          // function to calculate hours and minutes
{
  hours = hour() % 12;          // convert 24hr time to 12hr time
  minutes= int(minute()/5);       // number of 5min intervals
  if(minutes > 0 ) display = 0;     // reset hour display flag
}

void LEDminutes()         // function to turn on minute LEDs
{
  for(int i=0; i<minutes; i++)
  {               // set LED RGB values
    ringM.setPixelColor(i, ringM.Color(255*color, 255*(1-color), 0));
  }
  ringM.show();
}

void LEDhours()           // function to turn on hour LEDs
{
  for(int i=0; i<hours; i++)
  {
    ringH.setPixelColor(i, ringH.Color(255*(1-color), 255*color, 0));
  }
  ringH.show();
}

void rainbow()            // Adafruit rainbow function
{
  int cycle = 2;            // two cycles of colors
  for(long Pixel1Hue=0; Pixel1Hue<cycle*65536; Pixel1Hue += 256)
  {
    for(int i=0; i<ringM.numPixels(); i++)
    {
      int pixelHue = Pixel1Hue + (i * 65536L / ringM.numPixels());
      ringM.setPixelColor(i, ringM.gamma32(ringM.ColorHSV(pixelHue)));
    }
    ringM.show();
    delay(10);
  }
}

void phonering()            // function for phone ringtone
{
  for(int k=0; k<2; k++)        // two cycles
  {
    for(int j=0; j<2; j++)        //  of two rings
    {
      for(int i=0; i<5; i++)        //  with five repeats
      {
        playTone(300, 40);        // frequency 300Hz for 40ms
        playTone(350, 40);        // frequency 350Hz for 40ms
      }
      delay(200);           // 200ms delay between rings
    }
    delay(2000);            // 2s delay between cycles
  }
}

void playTone(int freq, int duration)
{
  analogWriteFreq(freq);        // frequency and duty cycle
  analogWrite(piezoPin, 512);       // to generate square wave
  delay(duration);          // for duration (ms)
  analogWrite(piezoPin, 0);       // disable PWM on pin
}
