/*******************************************************************************
 * Sketch name: nRF24L01 transmit signal with position data and display on webpage
 * Description: display on webpage GPS latitude and longitude values -  see Listing 12-4
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    12 - GPS tracking app with Google Maps
 ******************************************************************************/

#include <ESP8266WiFi.h>      // include ESP8266 Wi-Fi and
#include <ESP8266WebServer.h>     // webserver libraries
ESP8266WebServer server;      // associate server with library
char ssid[] = "xxxx";       // change xxxx to your Wi-Fi SSID
char password[] = "xxxx";     // change xxxx to your Wi-Fi password
#include "buildpage.h"        // webpage HTML code
#include <SoftwareSerial.h>     // include SoftwareSerial library
SoftwareSerial SoftSer(D4, D0);   // associate SoftSer with SoftwareSerial
#include <NMEAGPS.h>        // include NeoGPS library
NMEAGPS nmea;         // associate nmea with NMEAGPS lib
gps_fix gps;          // associate gps with NMEAGPS library
float GPSlat, GPSlong, GPSalt, GPSspd;    // real numbers for GPS location
int GPSsend = 0;            // GPS send counter
String json;
int count = 0;
String counter;         // counter increment every second
#include <SPI.h>          // include SPI library
#include <RF24.h>         // include RF24 library
RF24 radio(D3, D8);       // associate radio with RF24 library
byte addresses[ ][6] = {"12"};    // data pipe address
typedef struct          // define data structure to include
{
  float GPSlat;         // GPS latitude
  float GPSlong;          // GPS longitude
  float GPSalt;         // GPS altitude (m)
  float GPSspd;         // GPS ground speed (kmph)
  int sigCount;         // signal counter
} dataStruct;
dataStruct data;          // name the data structure as data
int LEDpin = D1;          // define LED pin
int LED = 0;          // LED turned off
int interval = 1;         // (interval+1)s between transmissions

void setup()
{
  Serial.begin(115200);       // define Serial Monitor baud rate
  SoftSer.begin(9600);        // serial connection to GPS module
  WiFi.begin(ssid, password);     // initialise Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500); // wait for Wi-Fi connect
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());   // display web server IP address
  server.begin();         // initialise server
  server.on("/", base);       // call base function as webpage loaded
  server.on("/GPSurl", GPSfunct);   // call GPSfunct with GPSurl loaded
  server.on("/countUrl", countFunct);
  delay(500);
  radio.begin();          // start radio
  radio.setChannel(50);       // set channel number,
  radio.setDataRate(RF24_2MBPS);    // baud rate and
  radio.setPALevel(RF24_PA_HIGH);   // power amplifier level
  radio.setAutoAck(true);     // set auto-acknowledge (default is true)
  radio.openWritingPipe(addresses[0]);  // initiate data transmit pipe
  radio.stopListening();      // set nRF24L01 module as transmitter
  pinMode(LEDpin, OUTPUT);      // define LEDpin as OUTPUT
}

void GPSfunct()       // function to transmit GPS position data
{
  while(nmea.available(SoftSer)>0)      // GPS data available
  {
    gps = nmea.read();          // latest satellite message
    if(gps.valid.location)        // validated GPS location
    {
      GPSlat = gps.latitude();
      GPSlong = gps.longitude();
      GPSsend++;            // increment GPS send counter
    }
    if(gps.valid.altitude) GPSalt = gps.altitude();   // altitude
    if(gps.valid.speed) GPSspd = gps.speed_kph();   // ground speed
    JsonConvert(GPSlat, GPSlong, GPSalt, GPSspd); // convert to JSON text
    server.send(200, "text/json", json);      // send JSON text to client
    Serial.println(json);
    if(GPSsend > interval)        // transmit every (interval+1)s
    {
      data.GPSlat = GPSlat;     // convert GPS readings to data structure
      data.GPSlong = GPSlong;
      data.GPSalt = GPSalt;
      data.GPSspd = GPSspd;
      data.sigCount++;          // increment signal counter
      radio.write(&data, sizeof(data));   // transmit signal as data structure
      GPSsend = 0;          // reset GPS send counter
      flashLED();
    }
  }
}
// function to convert data to JSON text
String JsonConvert(float val1, float val2, float val3, float val4)
{                 // start with open bracket
  json  = "{\"var1\": \"" + String(val1,4) + "\",";
  json += " \"var2\": \"" + String(val2,4) + "\","; // end with comma
  json += " \"var3\": \"" + String(val3)   + "\",";
  json += " \"var4\": \"" + String(val4)   + "\"}"; // end with close bracket
  return json;
}

void countFunct()         // function to increment counter
{             // and send value to client
  count++;
  counter = String(count);
  server.send (200, "text/plain", counter);
}

void flashLED()         // function to flash LED
{
  for (int i=0; i<4; i++)
  {
    LED = 1 - LED;        // alternate LED state four times
    digitalWrite(LEDpin, LED);    // ON – OFF – ON - OFF
    delay(50);
  }
}

void base()           // function to return HTML code
{
  server.send (200, "text/html", page);
}

void loop()
{
  GPSfunct();         // function to transmit GPS location data
  server.handleClient();      // manage HTTP requests
}
