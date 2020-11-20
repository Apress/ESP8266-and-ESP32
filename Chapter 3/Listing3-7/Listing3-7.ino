/*******************************************************************************
 * Sketch name: Paintpot with XPT2046 library
 * Description: "Painting" on touch screen - use your calibration parameters
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    3 - International weather station
 ******************************************************************************/

#include <Adafruit_ILI9341esp.h>      // include ILI9341esp and
#include <XPT2046.h>          //  XPT2046 libraries
int tftCS = D8;
int tftDC = D4;
int tftRST = D0;            // define screen and touch pins
int touchCS = D1;
int touchIRQ = D2;          // associate tft with ILI9341 lib
Adafruit_ILI9341 tft = Adafruit_ILI9341(tftCS, tftDC, tftRST);
XPT2046 touch(touchCS, touchIRQ);     // associate touch with XPT2046
String str;
uint16_t x, y;
int radius = 2;           // define paintbrush radius
unsigned int color;         // rotation: 0, 1, 2 or 3 refers to
int rotate = 1;           // rotation of 0, 90, 180 or 270

void setup()
{
  tft.begin();            // initialise ILI9341 screen
  setRotation();            // function for rotation parameters
  touch.setCalibration(1850,1800,320,300);  // calibration parameters
  clear();              // call function to reset screen
}

void loop()
{
  if (touch.isTouching()>0)       // if screen pressed
  {
    touch.getPosition(x, y);
    if(x>20) tft.fillCircle(x, y, radius, color); // draw point
    if(x>0 && x<20)         // select color from color palette
    {
           if(y>75 && y<95)   color = ILI9341_RED;
      else if(y>100 && y<120) color = ILI9341_YELLOW;
      else if(y>125 && y<145) color = ILI9341_GREEN;
      else if(y>150 && y<170) color = ILI9341_BLUE;
      else if(y>175 && y<195) color = ILI9341_WHITE;
// display selected color
      if(y>75 && y<195) tft.fillCircle(10, 50, 10, color);
      else if(y>215) clear();       // clear screen
    }
  }
}

void clear()            // function to reset screen
{
  tft.fillScreen(ILI9341_BLACK);      // fill screen
  tft.setTextColor(ILI9341_GREEN);      // set text color
  tft.setTextSize(2);         // set text size
  tft.setCursor(110,5);         // position cursor
  tft.print("Paintpot");        // screen title
  tft.fillRect(0,75,20,20, ILI9341_RED);
  tft.fillRect(0,100,20,20,ILI9341_YELLOW);
  tft.fillRect(0,125,20,20,ILI9341_GREEN);  // build color palette
  tft.fillRect(0,150,20,20, ILI9341_BLUE);
  tft.fillRect(0,175,20,20, ILI9341_WHITE);
  tft.drawCircle(10,225,10, ILI9341_WHITE); // select to clear screen
  tft.setCursor(25,217);
  tft.setTextColor(ILI9341_WHITE);
  tft.print("clear");
  color = ILI9341_WHITE;
}

void setRotation()        // function to set rotation parameters
{
  tft.setRotation(rotate);
  switch (rotate)
  {
    case 0:               // no rotation
       touch.begin(tft.width(), tft.height());    // portrait
       touch.setRotation(XPT2046::ROT0);      // connections at bottom
       break;
    case 1:               // rotation through 90
      touch.begin(tft.height(), tft.width());   // landscape
      touch.setRotation(XPT2046::ROT90);      // connections on right
    break;
    case 2:               // rotation through 180
      touch.begin(tft.width(), tft.height());   // portrait
      touch.setRotation(XPT2046::ROT180);     // connections at top
    break;
    case 3:               // rotation through 270
      touch.begin(tft.height(), tft.width());   // landscape
      touch.setRotation(XPT2046::ROT270);     // connections on left
    break;
  }
}
