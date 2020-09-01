/*******************************************************************************
 * Sketch name: ESP32 vs1053_ext library functions
 * Description: Demonstrate output of ESP32 vs1053_ext library functions
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    1 - Internet radio
 ******************************************************************************/
 
#include <vs1053_ext.h>          // include ESP32 VS1053_ext lib
#include <WiFi.h>           // include Wi-Fi library
int CS = 0;
int DCS = 2;            // define VS1053 decoder pins
int DREQ = 4;
VS1053 decoder(CS, DCS, DREQ);      // associate decoder with VS1053
char ssid[] = "xxxx";         // change xxxx to Wi-Fi ssid
char password[] = "xxxx";       // change xxxx to Wi-Fi password
int volume = 10;            // volume level

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  SPI.begin();            // initialise SPI bus
  WiFi.begin(ssid, password);       // initialise Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  decoder.begin();          // initialise VS0153 decoder
  decoder.setVolume(volume);        // set decoder volume level
  decoder.connecttohost("media-ice.musicradio.com:80/ClassicFMMP3");
}

void loop()
{
  decoder.loop();
}
void vs1053_showstation(const char * info)
{               // display radio station name
  Serial.print("Station:      ");
  Serial.println(info);
}
void vs1053_bitrate(const char * info)
{               // display streaming bit rate
  Serial.print("Bit rate:     ");
  Serial.println(String(info)+"kBit/s");
}
void vs1053_icyurl(const char * info)
{               // display radio station URL
  Serial.print("Homepage:     ");
  Serial.println(info);
}
void vs1053_showstreamtitle(const char * info)
{               // title of streamed track
  Serial.print("Stream title: ");
  Serial.println(info);
}
