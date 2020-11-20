/*******************************************************************************
 * Sketch name: Calibration of ILI9341 SPI TFT LCD screen
 * Description: Touch screen calibration when using TFT-eSPI library
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    3 - International weather station
 ******************************************************************************/
 
#include <TFT_eSPI.h>          // include TFT_eSPI library
TFT_eSPI tft = TFT_eSPI();        // associate tft with TFT-eSPI lib
uint16_t calData[5];          // calibration parameters
String str;

void setup()
{
  tft.init();           // initialise ILI9341 TFT screen
  tft.setRotation(1);         // landscape, connections on right
  tft.setTextFont(1);         // set text font and size
  tft.setTextSize(1);
  calibrate();            // call calibration function
}

void calibrate()        // function to calibrate ILI9341 TFT screen
{
  tft.fillScreen(TFT_BLACK);        // fill screen in black
  tft.setTextColor(TFT_WHITE, TFT_BLACK);   // set text color, white on black
  tft.setCursor(30, 0);         // move cursor to position (0, 30)
  tft.println("Touch corners as indicated");
  tft.calibrateTouch(calData, TFT_RED, TFT_BLACK, 15);  // calibrate screen
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 50);
  tft.setTextSize(2);
  tft.print("Calibration parameters");
  str = "";             // display calibration parameters
  for (int i=0; i<4; i++) str = str + String(calData[i])+",";
  str = str + String(calData[4]);
  tft.setCursor(0, 90);
  tft.print(str);
}
void loop()             // nothing in loop function
{}
