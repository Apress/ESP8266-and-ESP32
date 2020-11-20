/*******************************************************************************
 * Sketch name: nRF24L01 receive signal transmitted with Bluetooth
 * Description: transmit the received GPS latitude and longitude values
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    12 - GPS tracking app with Google Maps
 ******************************************************************************/

#include <BluetoothSerial.h>        // include Bluetooth library
BluetoothSerial SerialBT;       // associate SerialBT with library
#include <SPI.h>            // include SPI library
#include <RF24.h>           // include RF24 library
RF24 radio(2, 4);           // associate radio with RF24 lib
byte addresses[ ][6] = {"12"};      // data pipe address
typedef struct            // define data structure to include
{
  char GPSlat[10];          // character arrays for
  char GPSlong[10];         // GPS latitude and longitude
} dataStruct;
dataStruct data;            // name the data structure as data
int count = 0;            // received message counter
int textLen;
String text;
char c;

void setup()
{
  radio.begin();            // start radio
  radio.setChannel(50);         // set channel number
  radio.setDataRate(RF24_2MBPS);      //  baud rate
  radio.setPALevel(RF24_PA_HIGH);     //  and power amplifier
  radio.setAutoAck(true);       // set auto-acknowledge (default)
  radio.openReadingPipe(0, addresses[0]);   // initiate data receive pipe
  radio.startListening();       // nRF24L01 module as receiver
  SerialBT.begin("ESP32 Bluetooth");    // identify Bluetooth
}

void loop()
{
  if(radio.available())         // if signal received
  {
    radio.read(&data, sizeof(data));    // received signal to data structure
    count++;            // increment counter
    text = String(count) + "," + String(data.GPSlat) + "," + 
    String(data.GPSlong) + ",";     // build string of  position data
    textLen = text.length();
    for (int i=0; i<textLen; i++)
    {
      c = text[i];          // for each message character
      SerialBT.write(c);        // transmit to Bluetooth device
    }
  }
}
