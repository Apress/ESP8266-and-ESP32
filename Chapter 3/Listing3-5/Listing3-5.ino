/*******************************************************************************
 * Sketch name: Paintpot with TFT-eSPI library
 * Description: "Painting" on touch screen - use your calibration parameters
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    3 - International weather station
 ******************************************************************************/

#include <TFT_eSPI.h>          // include TFT-eSPI library
TFT_eSPI tft = TFT_eSPI();        // associate tft with TFT-eSPI lib
uint16_t calData[] = {450, 3400, 390, 3320, 3}; // calibration parameters
uint16_t x = 0, y = 0;
int radius = 2;           // define paintbrush radius
unsigned int color;

void setup()
{
  tft.init();           // initialise ILI9341 TFT screen
  tft.setRotation(1);         // landscape, connections on right
  tft.setTouch(calData);        // include calibration parameters
  clear();              // call function to reset screen
}

void loop()
{
  if (tft.getTouch(&x, &y)>0)       // if screen pressed
  {
    if(x>20) tft.fillCircle(x, y, radius, color); // draw point
    if(x>0 && x<20)         // select color from color palette
    {
           if(y>75 && y<95)   color = TFT_RED;
      else if(y>100 && y<120) color = TFT_YELLOW;
      else if(y>125 && y<145) color = TFT_GREEN;
      else if(y>150 && y<170) color = TFT_BLUE;
      else if(y>175 && y<195) color = TFT_WHITE;
// display selected color
      if(y>75 && y<195) tft.fillCircle(10, 50, 10, color);
      else if(y>215) clear();       // clear screen
    }
  }
}

void clear()            // function to reset screen

{
  tft.fillScreen(TFT_BLACK);        // fill screen
  tft.setTextColor(TFT_GREEN);      // set text color
  tft.setTextSize(2);         // set text size
  tft.setCursor(110,5);         // position cursor
  tft.print("Paintpot");        // screen title
  tft.fillRect(0,75,20,20, TFT_RED);
  tft.fillRect(0,100,20,20,TFT_YELLOW);
  tft.fillRect(0,125,20,20,TFT_GREEN);    // build color palette
  tft.fillRect(0,150,20,20, TFT_BLUE);
  tft.fillRect(0,175,20,20, TFT_WHITE);
  tft.drawCircle(10,225,10, TFT_WHITE);   // select to clear screen
  tft.setCursor(25,217);
  tft.setTextColor(TFT_WHITE);
  tft.print("clear");
  color = TFT_WHITE;
}
