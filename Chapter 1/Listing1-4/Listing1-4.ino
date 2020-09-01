/*******************************************************************************
 * Sketch name: Minimal internet radio
 * Description: Internet radio with ESP32 with only 21 lines of code
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    Chapter 1 - Internet radio
 ******************************************************************************/
 
#include <vs1053_ext.h>          // include ESP32 VS1053_ext
#include <WiFi.h>           //  and WiFi libraries
int CS = 0;
int DCS = 2;            // define VS1053 decoder pins
int DREQ = 4;
VS1053 decoder(CS, DCS, DREQ);      // associate decoder with VS1053
char ssid[] = "xxxx";         // change xxxx to Wi-Fi ssid
char password[] = "xxxx";       // change xxxx to Wi-Fi password

void setup()
{
  SPI.begin();            // initialise SPI bus
  WiFi.begin(ssid, password);       // initialise Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  decoder.begin();        // initialise VS0153 decoder
  decoder.setVolume(10);      // pre-set decoder volume level
  decoder.connecttohost("media-ice.musicradio.com:80/ClassicFMMP3");
}             // connect to pre-set radio station server

void loop()
{
  decoder.loop();
}
