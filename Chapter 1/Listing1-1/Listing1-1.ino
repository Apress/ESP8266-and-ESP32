/*******************************************************************************
 * Sketch name: Internet radio with volume and station switches
 * Description: Internet radio with ESP8266 development board
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    1 - Internet radio
 ******************************************************************************/

#include <VS1053.h>         // include VS1053 library
#include <ESP8266WiFi.h>        // include ESP8266WiFi library
int CS = D2;
int DCS = D3;           // define VS1053 decoder pins
int DREQ = D1; 
VS1053 decoder(CS, DCS, DREQ);      // associate decoder with VS1053
int statPin = D8;           // define switch pins for
int volPin = D4;            //  station and volume
WiFiClient client;          // associate client and library
char ssid[] = "xxxx";         // change xxxx to Wi-Fi ssid
char password[] = "xxxx";       // change xxxx to Wi-Fi password
const int maxStat = 4;          // number of radio stations
String stationName[] = {"1940 UK", "Bayern3", "ClassicFM", "BBC4"};
char * host[maxStat] = {"1940sradio1.co.uk",    // station host
"streams.br.de",
"media-ice.musicradio.com",
"bbcmedia.ic.llnwd.net"};
char * path[maxStat] ={"/stream/1/",      // station path
     "/bayern3_2.m3u",
     "/ClassicFMMP3",
     "/stream/bbcmedia_radio4fm_mf_q"};
int port[] = {8100,80,80,80};       // default station port is 80
unsigned char mp3buff[32];        // VS1053 loads data in 32 bytes
int station = 0;
int volume = 0;           // volume level 0-100
volatile int newStation = 2;        // station number at start up
volatile int newVolume = 80;        // volume at start up

void setup ()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  SPI.begin();            // initialise SPI bus
  decoder.begin();          // initialise VS1053 decoder
  decoder.switchToMp3Mode();        // MP3 format mode
  decoder.setVolume(volume);        // set decoder volume
  WiFi.begin(ssid, password);       // initialise Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("WiFi connected");     // wait for Wi-Fi connection
  pinMode(volPin, INPUT_PULLUP);    // switch pin uses internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(statPin), chan, RISING);
  attachInterrupt(digitalPinToInterrupt(volPin), vol, FALLING);
}         // define interrupts for changing station and volume

void loop()
{
  if(station != newStation)       // new station selected
  {
    station = newStation;       // display updated station name
    Serial.print("connecting to CH"); Serial.print(station);
    Serial.print(" ");Serial.println(stationName[station]);
    if(client.connect(host[station], port[station]))
    {               // connect to radio station URL
      client.println(String("GET ")+ path[station] + " HTTP/1.1");
      client.println(String("Host: ") + host[station]);
      client.println("Connection: close");
      client.println();         // new line is required
    }
  }
  if(volume != newVolume)       // change volume selected
  {
    volume = newVolume;         // display updated volume
    Serial.print("volume ");Serial.println(volume);
    decoder.setVolume(volume);      // set decoder volume
  }
  if(client.available() > 0)        // when audio data available
  {               // decode data 32 bytes at a time
    uint8_t bytesread = client.read(mp3buff, 32);
    decoder.playChunk(mp3buff, bytesread);
  }
}

IRAM_ATTR void chan()       // ISR to increment station number
{
  newStation++;
  if(newStation > maxStat-1) newStation = 0;  // stations numbered 0, 1, 2...
}

IRAM_ATTR void vol()          // ISR to increase volume
{
  newVolume = newVolume + 5;
  if(newVolume > 101) newVolume = 50;   // maximum volume is 100
}
