/*******************************************************************************
 * Sketch name: Pin definitions for ESP8266 and ESP32 development boards
 * Description: Code included at start of sketch for ESP8266 and ESP32
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    8 - Updating a webpage
 ******************************************************************************/

#ifdef ESP32
  #include <WebServer.h>        // include ESP32 library
  WebServer server (80);        //  and define LED pin
  int LEDpin = 23;
#elif ESP8266
  #include <ESP8266WebServer.h>     // include ESP8266 library
  ESP8266WebServer server;        //  and define LED pin
  int LEDpin = D3;
#else               // Arduino IDE error message
  #error "ESP8266 or ESP32 microcontroller only"
#endif
