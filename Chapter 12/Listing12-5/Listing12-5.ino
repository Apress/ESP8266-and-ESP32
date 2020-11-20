/*******************************************************************************
 * Sketch name: nRF24L01 receive signal with position data and display on OLED
 * Description: display received GPS data on OLED screen
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    12 - GPS tracking app with Google Maps
 ******************************************************************************/

#include <SPI.h>            // include SPI library
#include <RF24.h>           // include RF24 library
RF24 radio(D3, D8);         // associate radio with RF24 lib
byte addresses[ ][6] = {"12"};      // data pipe address
typedef struct            // define data structure to include
{
  float GPSlat;           // character arrays for
  float GPSlong;            // GPS latitude and longitude
  float GPSalt;           // GPS altitude (m)
  float GPSspd;           // GPS ground speed (kmph)
  int sigCount;           // signal counter
} dataStruct;
dataStruct data;            // name the data structure as data
float lagTime = 0;
#include <Adafruit_SSD1306.h>       // library 12864 OLED screen
int width = 128;            // OLED screen dimensions
int height = 64;
Adafruit_SSD1306 oled(width, height, &Wire, -1);
unsigned long lastTime, nowTime = 0;

void setup()
{
  radio.begin();            // start radio
  radio.setChannel(50);         // set channel number
  radio.setDataRate(RF24_2MBPS);      // baud rate
  radio.setPALevel(RF24_PA_HIGH);     // and power amplifier
  radio.setAutoAck(true);       // set auto-acknowledge (default)
  radio.openReadingPipe(0, addresses[0]);   // initiate data receive pipe
  radio.startListening();       // nRF24L01 as receiver
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();          // initialise OLED screen
  oled.setTextColor(WHITE);
  oled.setTextSize(1);          // text size of 68 pixels
  oled.display();
}

void loop()
{
  if(radio.available())         // if signal received
  {
    radio.read(&data, sizeof(data));    // set signal to data structure
    nowTime = millis();
    lagTime = (nowTime-lastTime)/1000.0;    //time since last signal received
    lastTime = nowTime;
    screen();           // call OLED screen function
  }
}

void screen()
{
  oled.clearDisplay();          // clear display
  oled.setCursor(0,0);          // position cursor
  oled.print(data.GPSlat,4);        // display GPS latitude
  oled.setCursor(65,0);         // and GPS longitude
  oled.print(data.GPSlong,4);
  oled.setCursor(0,10);
  oled.print("alt ");oled.print(data.GPSalt,1); // display GPS altitude
  oled.setCursor(65,10);        // and GPS speed
  oled.print("spd ");oled.print(data.GPSspd);
  oled.setCursor(0, 20);
  oled.print("lag ");oled.print(lagTime,2); // time since last signal
  oled.setCursor(65, 20);
  oled.print("chk ");oled.print(data.sigCount); // signals sent
  oled.display();
}
