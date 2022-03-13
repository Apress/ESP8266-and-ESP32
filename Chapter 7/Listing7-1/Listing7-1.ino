/*******************************************************************************
 * Sketch name: WLAN and LED functions
 * Description: Control two LEDs with buttons on webpage
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    7 - Wireless local area network
 ******************************************************************************/

#include <ESP8266WebServer.h>        // include ESP8266webserver lib
ESP8266WebServer server;        // associate server with library
char ssidAP[] = "ESP8266";        // WLAN SSID and password
char passwordAP[] = "12345678";
IPAddress local_ip(192,168,2,1);      // pre-defined IP address values
IPAddress gateway(192,168,2,1);
IPAddress subnet(255,255,255,0);
#include "buildpage.h"          // webpage HTML code
int LEDGpin = D7;           // define LED pins
int LEDRpin = D8;
int LEDR = LOW;           // default LED states
int LEDG = LOW;
int counter = 0;

void setup()
{
  WiFi.mode(WIFI_AP);         // Wi-Fi AP mode
  delay(1000);            // setup AP mode
  WiFi.softAPConfig(local_ip, gateway, subnet); //  predefined IP address
  WiFi.softAP(ssidAP, passwordAP);      // initialise Wi-Fi
  server.begin();           // initialise server
  server.on("/", base);         // load default webpage
  server.on("/LEDGurl", LEDGfunct);       // map URLs to functions:
  server.on("/LEDRurl", LEDRfunct);       //  LEDGfunct, LEDRfunct
  server.on("/zeroUrl", zeroFunct);     //  and zeroFunct
  pinMode(LEDGpin, OUTPUT);       // define LED pins as output
  pinMode(LEDRpin, OUTPUT);
}

void base()           // function to load default webpage
{               // and send HTML code to client
  server.send(200, "text.html", webcode(LEDG, LEDR, counter));
}

void LEDGfunct()          // function to change green LED state,
{               // increment counter and
  LEDG = !LEDG;           // send HTML code to client
  digitalWrite(LEDGpin, LEDG);
  counter++;
  server.send(200, "text/html", webcode(LEDG, LEDR, counter));
}

void LEDRfunct()          // function to change red LED state, 
{               // increment counter and
  LEDR = !LEDR;             // send HTML code to client
  digitalWrite(LEDRpin, LEDR);
  counter++;
  server.send(200, "text/html", webcode(LEDG, LEDR, counter)); 
}

void zeroFunct()          // function to zero counter
{               // and send HTML code to client
  counter = 0;
  server.send(200, "text/html", webcode(LEDG, LEDR, counter)); 
}

void loop()
{
  server.handleClient();        // manage HTTP requests
}
