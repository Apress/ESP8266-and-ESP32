/*******************************************************************************
 * Sketch name: Signal transmission to monitor nRF24L01 transceiver modules
 * Description: transmit "standard" signal to assess reception
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    12 - GPS tracking app with Google Maps
 ******************************************************************************/

#include <SPI.h>            // include SPI library
#include <RF24.h>           // include RF24 library
RF24 radio(D3, D8);         // associate radio with library
byte addresses[ ][6] = {"12"};      // data pipe address
typedef struct            // define data structure to include
{
  unsigned long counted;        // counter
  unsigned long mins;         // time (minute and second)
  unsigned long secs;
} dataStruct;
dataStruct data;
unsigned long lastTime, nowTime = 0;
int count = 0;
int mins = 0, secs = 0;

void setup()
{
  radio.begin();            // start radio
  radio.setChannel(50);         // set channel number,
  radio.setDataRate(RF24_2MBPS);      // data rate and
  radio.setPALevel(RF24_PA_HIGH);     // power amplifier
  radio.setAutoAck(false);        // set auto-acknowledge
  radio.openWritingPipe(addresses[0]);    // initiate data transmit pipe
  radio.stopListening();        // set nRF24L01 as transmitter
  mins = 0;
  secs = 0;
}

void loop()
{
  nowTime = millis();
  if(nowTime - lastTime > 1000)     // determine minutes
  {               //  and seconds
    secs++;
    if(secs > 59)           // after 60 seconds
    {
      secs = 0;           // reset second variable
      mins++;           // increment minute variable
    }
    data.counted = count;       // convert values to data structure
    data.mins = mins;
    data.secs = secs;
    count = 0;            // reset counter
    lastTime = nowTime;         // update time of "second"
  }
  radio.write(&data, sizeof(data));     // transmit signal
  count++;              // increment signal counter
}
