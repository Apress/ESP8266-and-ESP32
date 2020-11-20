/*******************************************************************************
 * Sketch name: LED rings clock with NTP time updating for ESP8266 board
 * Description: Clock with Wi-Fi connection and time obtained from NTP 
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    4 - Internet clock
 ******************************************************************************/

#include <Adafruit_NeoPixel.h>
int LEDpinL = D2;          // left ring to display hours
int LEDpinR = D3;         // right ring to display minutes
int piezoPin = D1;        // Piezo transducer pin
int LEDnumber = 12;
unsigned long interval = 1000;
int color = 0;
String text;
Adafruit_NeoPixel ringM(LEDnumber, LEDpinR, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ringH(LEDnumber, LEDpinL, NEO_GRB + NEO_KHZ800);
#include <ESP8266WiFi.h>      // library to connect to Wi-Fi network
#include <ESP8266WebServer.h>     // library for webserver functionality
ESP8266WebServer server;  // associate server with ESP8266WebServer library
char ssid[] = "xxxx";       // replace xxxx with Wi-Fi ssid
char password[] = "xxxx";     // replace xxxx with Wi-Fi password
#include <NTPtimeESP.h>       // include NTPtime library
// associate NTP with NTPtime library
NTPtime NTP("uk.pool.ntp.org");     // UK server pool for NTPtime
strDateTime dateTime;
unsigned long epoch;          // Unix epoch time
#include <TimeLib.h>          // include Time library
int minutes, hours;
int display = 0;

void setup()
{
  pinMode(piezoPin, OUTPUT);        // Piezo transducer pin as output
  getEpoch();           // get Epoch time from NTP
  getTime();
  ringH.begin();
  ringH.setBrightness(1);
  LEDhours();
  ringH.show();
  ringM.begin();
  ringM.setBrightness(1);
  LEDminutes();
  ringM.show();
}

void loop()
{
  getTime();
  if(minutes == 0 && display ==0)
  {
    display = 1;
    phonering();
    rainbow();
    ringM.clear();
    ringM.show();
    color = 1 - color;
    ringH.clear();
    ringH.show();
    LEDhours();
  }
  if(minutes > 0) LEDminutes();
  if(hours == 8 && minutes == 30) getEpoch();   // update Epoch time
  delay(interval);          // delay between time calculations
}

void getTime()            // as in Listing 4-4

void getEpoch()           // function to get NTP time
{
  WiFi.begin(ssid, password);       // initialise Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500); // wait for Wi-Fi connect
  epoch = 0;
  for (int i=0; i<5; i++)       // five attempts to access NTP
  {
    delay(500);           // delay between Wi-Fi connect
    dateTime = NTP.getNTPtime(0, 1);    //  and sourcing NTP data
    if(dateTime.valid)
    {
      epoch = dateTime.epochTime;     // NTP Epoch time obtained
      i = 5;            // stop attempting connect to NTP
    }
  }
  WiFi.disconnect(true);        // disconnect Wi-Fi connection
  WiFi.mode(WIFI_OFF);          // switch off Wi-Fi connection
  setTime(epoch);           // set internal clock to Epoch time
}

void LEDminutes()           // as in Listing 4-4

void LEDhours()           // as in Listing 4-4

void rainbow()            // as in Listing 4-4

void phonering()            // as in Listing 4-4

void playTone(int freq, int duration)   // as in Listing 4-4
