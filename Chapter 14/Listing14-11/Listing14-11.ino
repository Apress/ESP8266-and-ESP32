/*******************************************************************************
 * Sketch name: Characterising LoRa message reception
 * Description: received message deisplayed on OLED screen
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
int width = 128;            // OLED dimensions
int height = 64;
#include <Wire.h>           // include libraries for OLED
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 oled(width, height, &Wire, -1);  // Reset pin not required
String packet;
int RSSI, packetSize, interval;
float SNR;
unsigned long lastTime = 0;

void setup()
{
  digitalPinToInterrupt(DIO0);      // set pin as interrupt
  LoRa.setPins(CSS, RST, DIO0);     // define LoRa module pins
  while (!LoRa.begin(433E6)) delay(500);    // 433MHz transmission
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // OLED display I2C address
  oled.setTextColor(WHITE);       // set font color
  oled.setTextSize(2);          // text size 1216 pixels
  oled.display();
}

void loop()
{
  packetSize = LoRa.parsePacket();      // detect received packet
  if (packetSize > 0)
  {
    interval = round((millis() - lastTime)/1000); // interval (s)
    lastTime = millis();        // update message time
    packet = "";            // read packet
    while(LoRa.available()) packet = packet + ((char)(LoRa.read()));
    RSSI = LoRa.packetRssi();
    SNR = LoRa.packetSnr();       // signal : noise
    screen();           // OLED display function
    LoRa.beginPacket();         // start LoRa transmission
    LoRa.print("recv " + packet);     // send packet
    LoRa.endPacket();         // close LoRa transmission
  }
}

void screen()           // function for OLED display
{
    oled.clearDisplay();
    oled.setCursor(0,0);
    oled.print("lag  ");oled.println(interval); // display time since last message
    oled.print("RSSI ");oled.println(RSSI); // display interval, RSSI and SNR
    oled.print("SNR  ");oled.println(SNR);
    oled.print("msg  ");oled.print(packet); // display received message
    oled.display();
}
