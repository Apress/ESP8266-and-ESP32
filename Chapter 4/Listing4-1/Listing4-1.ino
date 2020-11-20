/*******************************************************************************
 * Sketch name: Sweeping colors along an RGB LED strip
 * Description: Demonstration of controlling a WS2812 RGB LED strip
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    4 - Internet clock
 ******************************************************************************/

#include <Adafruit_NeoPixel.h>    // include Adafruit NeoPixel library
int LEDpin = D1;          // define data pin
int LEDnumber = 30;       // number of LEDS in strip
              // associate strip with NeoPixel library
Adafruit_NeoPixel strip(LEDnumber, LEDpin, NEO_GRB + NEO_KHZ800);
// color white,red,lime,blue,yellow,cyan,magenta,grey,maroon,olive,green,purple,teal,navy
int R[ ] = {255,255,  0,  0,255,  0,255,128,128,128,  0,128,  0,  0};
int G[ ] = {255,  0,255,  0,255,255,  0,128,  0,128,128,  0,128,  0};
int B[ ] = {255,  0,  0,255,  0,255,255,128,  0,  0,  0,128,128,128};
uint32_t color;         // color is 32-bit or unsigned long

void setup()
{
  strip.begin();          // initialise LED strip
  strip.setBrightness(10);      // define LED brightness (1 to 255)
  strip.show();           // sets all pixels to "off" as no color set
}

void loop()
{
  for (int i=0; i<14; i++)      // cycle through the RGB colors
  {
    color = strip.Color(R[i],G[i],B[i]);  // convert RGB values to 32-bit number
    sweep(color, 40);       // sweep color through the LED strip
  }
  rainbow(3, 10);         // rainbow colors for three cycles 
}             // with a 10ms time lag for each color

void sweep(uint32_t color, int lag)   // color sweep function
{
  for (int i=0; i<strip.numPixels(); i++) // for each LED in strip
  {
    strip.setPixelColor(i, color);    // set the LED color
//  strip.setPixelColor(strip.numPixels()-i-1, color);  // reverse direction
    strip.show();         // update LED strip
    delay(lag);         // time lag between color changes
  }
}

void rainbow(int cycle, int lag)    // from Adafruit NeoPixel>strandtest
{
  for (long Pixel1Hue = 0; Pixel1Hue < cycle*65536; Pixel1Hue += 256)
  {
    for (int i=0; i<strip.numPixels(); i++)
    {
      int pixelHue = Pixel1Hue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();         // update LED states and colors
    delay(lag);
  }
}
