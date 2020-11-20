/*******************************************************************************
 * Sketch name: Transmitting LoRa module with feedback
 * Description: transmitted and feedback messages deisplayed on OLED screen
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    14 - ESP-NOW and LoRa communication
 ******************************************************************************/

#include <SPI.h>            // include SPI and
#include <LoRa.h>           //  LoRa libraries
int CSS = D8;           // define SX1278 pins
int RST = -1;           // RESET pin
int DIO0 = D2;            // interrupt pin
#include <Wire.h>           // include libraries for OLED
#include <Adafruit_SSD1306.h>
int width = 128;            // OLED dimensions
int height = 32;            // associate oled with library
Adafruit_SSD1306 oled(width, height, &Wire, -1);
int counter = 0;
unsigned long lastTime;
String packet, recv;
int packetSize;           // size of received message

void setup()
{
  digitalPinToInterrupt(DIO0);      // set pin as interrupt
  LoRa.setPins(CSS, RST, DIO0);     // define LoRa module pins
  LoRa.setSpreadingFactor(9);       // define spreading factor
  LoRa.setSignalBandwidth(62.5E3);      // set bandwidth to 62.5kHz
  while (!LoRa.begin(433E6)) delay(500);    // 433MHz transmission
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // OLED display I2C address
  oled.setTextColor(WHITE);       // set font color
  oled.setTextSize(2);          // text size 1216 pixels
  oled.display();
}

void loop()
{
  if(millis() - lastTime > 5000)      // 5s transmission interval
  {
    screen();           // OLED display function
    packet = String(counter);       // create packet
    LoRa.beginPacket();         // start LoRa transmission
    LoRa.print(packet);         // send packet
    LoRa.endPacket();         // close LoRa transmission
    counter++;            // increment counter
    lastTime = millis();        // update transmission time
  }
  packetSize = LoRa.parsePacket();      // detect received packet
  if (packetSize > 0)
  {
    recv = "";            // read packet
    while(LoRa.available()) recv = recv + ((char)(LoRa.read()));
    screen();           // OLED display function
  }
}+

void screen()           // function for OLED display
{
  oled.clearDisplay();
  oled.setCursor(0,0);
  oled.print("sent ");oled.println(packet); // transmitted value
  oled.print(recv);         // received message
  oled.display();
}
