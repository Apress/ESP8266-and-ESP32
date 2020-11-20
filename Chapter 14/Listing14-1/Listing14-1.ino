/*******************************************************************************
 * Sketch name: MAC address
 * Description: obtain microcontroller MAC address
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    14 - ESP-NOW and LoRa communication
 ******************************************************************************/

#include <ESP8266WiFi.h>        // include Wi-Fi library

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  Serial.println(WiFi.macAddress());    // get MAC address
}

void loop()             // nothing in loop function
{}
