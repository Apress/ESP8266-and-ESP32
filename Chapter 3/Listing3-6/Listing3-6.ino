/*******************************************************************************
 * Sketch name: Calibration of ILI9341 SPI TFT LCD screen for XPT2046 library
 * Description: Touch screen calibration when using XPT2046 library
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
uint16_t p,q, np, nq;         // co-ordinates of touched point
String str;

void setup()
{
  tft.begin();            // initialise ILI9341 screen
  tft.setRotation(0);         // portrait, connections at bottom
  touch.begin(tft.height(), tft.width());   // XPT2046 orientated y, x
  touch.setRotation(XPT2046::ROT0);     // no screen rotation
  tft.fillScreen(ILI9341_BLACK);      // fill screen
  tft.setTextColor(ILI9341_WHITE);      // set text color
  tft.setTextSize(1);         // set text size
  tft.setCursor(0, 100);        // position cursor
  str = "width: "+String(tft.width())+", height: "+String(tft.height());
  tft.print(str);
  calibrate();            // calibrate touch screen
}

void calibrate()            // function to calibrate screen
{
  uint16_t x1,y1,x2,y2,i1,j1,i2,j2;     // uint16_t is unsigned integer
  tft.setCursor(0, 50);         // position cursor
  tft.print("press screen on crosses");
  touch.getCalibrationPoints(x1, y1, x2, y2); // values pre-set in library at 20
  getPoints(x1, y1, i1, j1);        // function to get touch position
  delay(500);
  getPoints(x2, y2, i2, j2);        // get second touch position
  touch.setCalibration(i1, j1, i2, j2);     // string with parameters
  str = String(i1)+","+String(j1)+","+String(i2)+","+String(j2);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(0, 175);
  tft.print("calibration parameters");
  tft.setCursor(0, 200);
  tft.setTextSize(2);         // reset text size
  tft.print(str);           // display calibration parameters
}

void getPoints(uint16_t x, uint16_t y, uint16_t &i, uint16_t &j)
{
  marker(y, x, ILI9341_WHITE);      // draw white cross on screen
  while (!touch.isTouching()>0) delay(10);  // wait for screen touch
  touch.getRaw(i, j);
  marker(y, x, ILI9341_BLACK);      // over-write cross
  touch.getPosition(p, q);        // get position of screen touch
  np = (150.0-p)*240.0/145.0;       // transform from touch to tft
  nq = (115.0-q)*320.0/100.0;
  tft.fillCircle(np, nq, 2, ILI9341_GREEN); // indicated touch position
}

void marker(unsigned short x, uint16_t y, int col)
{
  tft.setTextColor(col);        // set marker color
  tft.drawLine(x-8, y, x+8, y, col);    // draw horizontal line
  tft.drawLine(x, y-8, x, y+8, col);    // draw vertical line
}

void loop()             // nothing in loop function
{}
