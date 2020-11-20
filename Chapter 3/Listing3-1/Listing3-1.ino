/*******************************************************************************
 * Sketch name: Display text and shapes
 * Description: Demonstration of displaying text and images on screen
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    3 - International weather station
 ******************************************************************************/

 #include <Adafruit_ILI9341.h>       // include ILI9341 library
int tftCS = D8;           // screen chip select pin
int tftDC = D4;           // data command select pin
int tftRST = D0;            // reset pin
// associate tft with ILI9341 lib
Adafruit_ILI9341 tft = Adafruit_ILI9341(tftCS, tftDC, tftRST);
String texts[] =            // color names
  {"BLUE","RED","GREEN","CYAN","MAGENTA","YELLOW","WHITE","GREY"};
unsigned int colors[ ] =        // color codes
  {ILI9341_BLUE, ILI9341_RED, ILI9341_GREEN, ILI9341_CYAN,
   ILI9341_MAGENTA, ILI9341_YELLOW, ILI9341_WHITE, ILI9341_LIGHTGREY};
String text;
unsigned int color, chkTime;

void setup()
{
  tft.begin();            // initialise screen
  tft.setRotation(2);         // portrait, connections at top
  tft.fillScreen(ILI9341_BLACK);      // fill screen in black
  tft.drawRect(0,0,239,319,ILI9341_WHITE);  // draw white frame line
  tft.drawRect(1,1,237,317,ILI9341_WHITE);  //  and second frame line
  tft.setTextSize(4);         // set text size
}

void loop()
{
  tft.fillRect(2,2,235,314,ILI9341_BLACK);  // clear screen apart from frame
  for (int i=0; i<8; i++)       // for each color
  {
    color = colors[i];          // set color
    text = texts[i];          // set text for color
    tft.setTextColor(color);        // set text color
    tft.setCursor(20,40*i+2);       // position cursor
    tft.print(text);          // print color text (name)
    delay(250);           // delay 250ms between colors
  }
  for (int i=0; i<8; i++)       // for each color
  {
    color = colors[i];
    text = texts[i];
    tft.fillRect(2,2,235,314,ILI9341_BLACK);
    tft.setCursor(20,25);       // cursor to position (20, 25)
    tft.setTextColor(color);  
    tft.print(text);          // draw filled-in triangle
    if ((i+1) % 3 == 0) tft.fillTriangle(20,134,64,55,107,134,color);
                // draw open rectangle
    else if ((i+1) % 2 == 0) tft.drawRect(20,55,88,80,color);
    else tft.fillCircle(64,95,39,color);    // draw filled-in circle
    delay(250);
  }
  tft.fillRect(2,2,235,314,ILI9341_BLACK);
  tft.drawLine(2,158,236,158,ILI9341_RED);  // draw horizontal RED line
  delay(250);
}
