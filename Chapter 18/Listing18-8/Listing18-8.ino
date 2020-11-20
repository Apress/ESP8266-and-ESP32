/*******************************************************************************
 * Sketch name: Solar panel and battery meter
 * Description: Measure current with INA219 module with graphic display
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    18 - Measuring electricity
 ******************************************************************************/

#include <Adafruit_INA219.h>        // define Adafruit INA219 library
Adafruit_INA219 inaSolar;     // associate inaSolar and inaBatt with lib
Adafruit_INA219 inaBatt(0x41);    // I2C address 0x41, 0x40 is default
#include <Adafruit_ST7735.h>        // include the ST7735 library
int TFT_CS = D8;            // ST7735 screen chip select pin
int DCpin = D3;           // ST7725 screen DC pin
int RSTpin = D4;            // ST7735 screen reset pin
// associate tft with Adafruit_ST7735 lib
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, DCpin, RSTpin);
int batVPin = A0;           // battery voltage reading pin
int scalePin = D6;          // scale pin for interrupt
unsigned int BLACK = 0x0000;
unsigned int YELLOW = 0xFFE0;
unsigned int GREEN = 0x07E0;        // HEX codes for colors
unsigned int RED = 0xF800;
unsigned int WHITE = 0xFFFF;
int maxRead = 99;           // maximum readings per screen
int n = 0;              // reading number
int maxVal = 0;           // initial maximum and minimum
int minVal = 1000;          //   for battery output
int initialY = 60;          // initial maximum of Y axis
int alterY = 300;           // alternative maximum of Y axis
volatile int maxY;          // actual maximum Y axis
float maxV = 4.80;          // maximum and minimum value
float minV = 2.40;          //   for battery voltage
float labelV = (maxV-minV)/6.0;     // 6 labels on battery voltage axis
int delayTime = 5;          // delay (s) between readings
float energy = 0;           // cumulative energy output
float batt, solar;          // battery and solar current
float battV;            // battery voltage
float battS, solarS, battVS;        // scaled values for graph
int labelY, newmaxY, batDirect;

void setup()
{
  inaSolar.begin();         // initialise INA219 modules
  inaBatt.begin();
  inaSolar.setCalibration_16V_400mA();    // 16V, 400mA range
  inaBatt.setCalibration_16V_400mA();
  tft.initR(INITR_BLACKTAB);        // initialise ST7735 screen
  tft.fillScreen(BLACK);        // clear screen
  tft.setRotation(3);         // orientate ST7735 screen
  tft.drawRect(0,0,160,128,WHITE);      // draw white frame line
  tft.drawRect(1,1,158,126,WHITE);      //  and second frame line
  maxY = initialY;          // set graph axes
  newmaxY = initialY;
  graph();              // call graph function
  pinMode(scalePin, INPUT_PULLUP);      // set pin state HIGH
  attachInterrupt(digitalPinToInterrupt(scalePin), scale, FALLING);
}

void loop()
{
  if(newmaxY != maxY)         // change graph Y axis
  {
    newmaxY = maxY;
    graph();            // call graph function
    n = 0;              // set number of readings to zero
  }
  n++;              // increment reading number
  if(n > maxRead)
  {               // new screen if n > maximum
    graph();
    n = 0;
  }
  batt = inaBatt.getCurrent_mA();     // battery current output
  if(batt < minVal) minVal = batt;      // update minimum and
  if(batt > maxVal) maxVal = batt;      //  maximum battery current
  screenVal(WHITE, minVal, 5, "min");   // display minimum and
  screenVal(WHITE, maxVal, 85, "max");    //   maximum battery current
  solar = inaSolar.getCurrent_mA();     // solar panel current reading
  solarS = 110-100.0*solar/maxY;      // scale solar panel reading
  tft.fillCircle(25+n, solarS, 1, WHITE);   // plot scaled solar panel reading
  if(batt < 0)
  {               // when battery charging
    screenVal(RED, batt, 35, "");     // change line color to RED
    battS = 110+100.0*batt/maxY;      // scale battery reading
    tft.fillCircle(25+n, battS, 1, RED);    // plot scaled battery reading
  }
  else
  {               // when battery discharging
    screenVal(GREEN, batt, 35, "");     // change line color to GREEN
    battS = 110-100.0*batt/maxY;
    tft.fillCircle(25+n, battS, 1, GREEN);
  }               // double battery reading
  battV = 2.0*analogRead(batVPin);      //   due to voltage divider
  battV = 3.2 * battV /1024.0;      // battery voltage
  screenVal(YELLOW, battV*100, 105, "");    // change color to YELLOW
  battVS = 110-100.0*(battV-minV)/(maxV-minV);  // scale battery voltage
  tft.fillCircle(25+n, battVS, 1, YELLOW);  // plot scaled battery voltage
  detail();             // function for solar and energy
  delay(delayTime * 1000);        // delay (s) between readings
}

void graph()            // function to draw graph
{
  labelY = maxY/6;          // 7 labels on each Y axis
  tft.fillRect(2,2,156,124,BLACK);      // fill screen in BLACK
  tft.drawRect(25,10,100,100,GREEN);    // graph rectangle in GREEN
  for (int i=0; i<7; i++)       // label y axis
  {
    tft.setCursor(5,10+i*(100/6-1));    // position labels
    tft.setTextColor(WHITE, BLACK);
    tft.print(maxY-i*labelY);       // left-hand side Y axis value
    tft.setCursor(130,10+i*(100/6-1));
    tft.setTextColor(YELLOW, BLACK);
    tft.print(maxV-i*labelV,2);     // right-hand side Y axis value
  }
  for (int j=0; j<5; j++)       // draw 5 dashed lines on graph
  for (int i=0; i<33; i++) tft.drawPixel(25+3*i, 28+j*100/6, YELLOW);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(30, 15);        // headings for solar and energy
  tft.print("sol");
  tft.setCursor(75, 15);
  tft.print("mAh");
}

void screenVal(unsigned int color, int val, int x, String text)
{           // function to display text and value below graph
  tft.setTextSize(1);
  tft.setTextColor(color, BLACK);
  tft.setCursor(x, 115);        // row number 115
  tft.print(text);
  tft.setCursor(x + 25, 115);       // position in row
  if(x == 5) tft.fillRect(30,115,20,8,BLACK); // over-write previous value
  else if(x == 35) tft.fillRect(60,115,20,8,BLACK);
  else if(x == 85) tft.fillRect(110,115,20,8,BLACK);
  tft.setTextColor(color, BLACK);
  if(x != 105) tft.print(val);      // print new value
  else tft.print(val/100.0,2);      // 2DP for battery voltage
}

IRAM_ATTR void scale()          // ISR to change Y axis scale
{
  if(newmaxY == alterY) maxY = initialY;
  else maxY = alterY;
}

void detail()
{               // battery energy output
  energy = energy + battV * delayTime * batt /3600.0; // energy (mAh)
  tft.setTextColor(WHITE, BLACK);
  tft.fillRect(50,15,15,8,BLACK);     // overlay if fewer digits
  tft.setCursor(50,15);
  tft.print(solar,0);         // solar panel current output
  tft.fillRect(100,15,20,8,BLACK);
  tft.setCursor(100,15);
  tft.print(energy,0);          // battery current output
}
