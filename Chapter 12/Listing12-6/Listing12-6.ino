/*******************************************************************************
 * Sketch name: Channel scanning
 * Description: determine activity on each scanned channel
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    12 - GPS tracking app with Google Maps
 ******************************************************************************/

#include <SPI.h>            // include SPI library
#include <RF24.h>           // include RF24 library
RF24 radio(D3, D8);         // associate radio with library
const int nChan = 126;          // 126 channels available
int chan[nChan];            // store counts per channel
int nScan = 100;            // number of scans per channel
int scan;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  radio.begin();              // start radio
}

void loop()
{
  for (int i=0;i<nChan;i++)         // for each channel
  {
    chan[i] = 0;              // reset counter
    for (scan=0; scan<nScan; scan++)      // repeat scanning
    {
      radio.setChannel(i);          // define channel
      radio.startListening();
      delayMicroseconds(128);         // listen for 128s
      radio.stopListening();
      if(radio.testCarrier()>0) chan[i]=chan[i]+1;  // a carrier on the channel
    }
    delay(1);             // avoid watchdog reset
  }
  for (int i=0; i<nChan; i++)         // for each channel
  {
    if(i%10 == 0) Serial.print("|");
    Serial.print(chan[i], HEX);       // display carrier activity
  }               // format in HEX for values <16
  Serial.println();           // new line
}
