/*******************************************************************************
 * Sketch name: Pin definitions for ESP8266 and ESP32 development boards
 * Description: Code included at start of sketch for ESP8266 and ESP32
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    7 - Wireless local area network
 ******************************************************************************/

#ifdef ESP8266
  #include <ESP8266WebServer.h>      // include ESP8266 library
  ESP8266WebServer server;
  int LEDGpin = D7;         // define LED pins
  int LEDRpin = D8;
#elif ESP32
  #include <WebServer.h>        // include ESP32 library
  WebServer server (80);
  int LEDGpin = 26;         // define LED pins
  int LEDRpin = 25;
#else               // Arduino IDE error message
  #error "ESP8266 or ESP32 microcontroller only"
#endif
