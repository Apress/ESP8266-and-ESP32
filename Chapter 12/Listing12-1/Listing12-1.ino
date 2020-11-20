/*******************************************************************************
 * Sketch name: nRF24L01 transmit signal with position data for ESP8266 board
 * Description: transmit GPS latitude and longitude values to receiving nRF24L01
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    12 - GPS tracking app with Google Maps
 ******************************************************************************/

#include <SoftwareSerial.h>      // include SoftwareSerial library
SoftwareSerial SoftSer(D4, D0);   // associate SoftSer with SoftwareSerial
#include <NMEAGPS.h>        // include NeoGPS library
NMEAGPS nmea;         // associate nmea and gps
gps_fix gps;          //  with NMEAGPS library
float GPSlat, GPSlong;        // real numbers for GPS location
int GPSsend = 0;          // GPS send counter
#include <SPI.h>          // include SPI library
#include <RF24.h>         // include RF24 library
RF24 radio(D3, D8);       // associate radio with RF24 library
byte addresses[ ][6] = {"12"};    // data pipe address
typedef struct          // define data structure to include
{
  char GPSlat[10];        // character arrays for
  char GPSlong[10];       //  GPS latitude and longitude
} dataStruct;
dataStruct data;          // name the data structure as data
int interval = 2;         // interval (s) between GPS transmissions

void setup()
{             // Serial connection to GPS module
  SoftSer.begin(9600);        // SoftwareSerial baud rate
  delay(500);
  radio.begin();            // start radio
  radio.setChannel(50);         // set channel number,
  radio.setDataRate(RF24_2MBPS);      //  baud rate
  radio.setPALevel(RF24_PA_HIGH);     //  and power amplifier
  radio.setAutoAck(true);       // set auto-acknowledge (default)
  radio.openWritingPipe(addresses[0]);    // initiate data transmit pipe
  radio.stopListening();        // nRF24L01 as transmitter
}

void loop()
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
    if(GPSsend > interval)        // transmit every (interval+1)s
    {         // convert number to string and then to character array
      String(GPSlat,6).toCharArray(data.GPSlat,10);
      String(GPSlong,6).toCharArray(data.GPSlong,10);
      radio.write(&data, sizeof(data));   // transmit signal as data structure
      GPSsend = 0;          // reset GPS send counter
    }
  }
}
