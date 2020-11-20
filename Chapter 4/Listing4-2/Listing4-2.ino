/*******************************************************************************
 * Sketch name: RGB LED strip and sound
 * Description: WS2812 RGB LED strip respondiing to MAX4466 electret microphone
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    4 - Internet clock
 ******************************************************************************/

#include <Adafruit_NeoPixel.h>      // include Adafruit NeoPixel lib
int LEDpin = D1;            // define data pin
int LEDnumber = 30;         // number of LEDS in strip
// associate strip with NeoPixel lib
Adafruit_NeoPixel strip(LEDnumber, LEDpin, NEO_GRB + NEO_KHZ800);
// colors red, orange, yellow, green, blue, indigo, violet
int R[ ] = {255, 255, 255,   0,   0,  75, 238};
int G[ ] = {  0, 102, 153, 255,   0,   0, 130};
int B[ ] = {  0,   0,   0,   0, 255, 130, 238};
uint32_t color;           // color is 32-bit or unsigned long
#include <RunningMedian.h>        // include Running Median lib
RunningMedian samples = RunningMedian(7);   // median filter sample size of 7
int sound, soundMax, soundMin, peak2peak, median, LEDs, val;
int adjustVol = 1;          // initial volume and baseline
int baseline = 0;
int soundTime = 50;         // sample sound time (ms)
unsigned long startTime;

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  Serial.println("\nenter volume adjustment , baseline");
  strip.begin();            // initialise LED strip
  strip.setBrightness(10);        // define LED brightness (1 - 255)
  strip.show();         // sets all pixels to "off" as no color set
}

void loop()
{
  while(Serial.available()>0)       // adjust volume and baseline
  {
    adjustVol = Serial.parseInt();      // convert Serial buffer to integers
    baseline = Serial.parseInt();
    Serial.print("\nVolume ");Serial.print(adjustVol);
    Serial.print("\tBaseline ");Serial.println(baseline);
  }               // get new peak to peak value
  getSound();           // number of LEDs to turn on
  LEDs = adjustVol*(median/1024.0)*LEDnumber-baseline;
  strip.clear();            // turn all LEDs off
  delay(10);            // allow time to switch off LEDs
  for (int i=0; i<LEDs; i++)
  {
    val = i/4;            // four LEDs have the same color
    color = strip.Color(R[val],G[val],B[val]);  // convert RGB values to color
    strip.setPixelColor(i, color);      // set the LED color
  }
  strip.show();           // update LED states and colors
}

void getSound()           // function for peak to peak value
{
  soundMax = 0;           // initial values for minimum and
  soundMin = 1024;          //  maximum sound values
  startTime = millis();         // start of sampling period
  while(millis() - startTime < soundTime)   // during sampling period
  {               //  determine minimum and
    sound = analogRead(A0);       //  maximum sound values
    if(sound > soundMax) soundMax = sound;
    else if(sound < soundMin) soundMin = sound;
  }
  peak2peak = soundMax - soundMin;      // peak to peak value
  samples.add(peak2peak);
  median = samples.getMedian();     // median peak to peak value
}
