/*******************************************************************************
 * Sketch name: Signal reception to monitor nRF24L01 transceiver modules
 * Description: assess reception of "standard" transmitted signal
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
  unsigned long sent;         // sent signals
  unsigned long mins;         // time (minute and second)
  unsigned long secs;
} dataStruct;
dataStruct data;
#include <Adafruit_SSD1306.h>       // library 12864 OLED screen
int width = 128;            // OLED screen dimensions
int height = 64;
Adafruit_SSD1306 oled(width, height, &Wire, -1);
const int Nval = 20;          // size of circular buffer
int pkts[Nval], sents[Nval];        // arrays for circular buffer
int N = 0, pkt = 0;
unsigned long sumPkt = 0, sumSent = 0;
float avgPkt, avgSent;
unsigned long lastTime, nowTime = 0;    // variables to store time values

void setup()
{
  radio.begin();            // start radio
  radio.setChannel(50);         // set channel number
  radio.setDataRate(RF24_2MBPS);      // data rate
  radio.setPALevel(RF24_PA_HIGH);     // and power amplifier
  radio.setAutoAck(false);        // set auto-acknowledge
  radio.openReadingPipe(0, addresses[0]);   // initiate data receive pipe
  radio.startListening();       // set nRF24L01 as transmitter
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();          // initialise OLED screen
  oled.setTextColor(WHITE);
  oled.display();
  data.sent = 0;
  for (int i=0; i<10; i++)        // set circular buffer arrays to zero
  {
    pkts[i] = 0;
    sents[i] = 0;
  }
}

void loop()
{
  if(radio.available())         // signal available
  {
    radio.read(&data, sizeof(data));    // read signal and
    pkt++;              // increment signal counter
  }
  nowTime = millis();
  if(nowTime - lastTime > 1000)     // update values every second
  {
    sumPkt = sumPkt - pkts[N];      // subtract oldest value from sum
    sumPkt = sumPkt + pkt;        // add current value to sum
    pkts[N] = pkt;          // update circular buffer
    sumSent = sumSent - sents[N];
    sumSent = sumSent + data.sent;
    sents[N] = data.sent;
    N++;
    if(N > Nval-1) N = 0;       // back to "start" of circular buffer
    avgPkt = 1.0*sumPkt / Nval;     // calculate moving averages
    avgSent = 1.0*sumSent / Nval;
    screen();           // call OLED screen function
    pkt = 0;
    data.sent = 0;
    lastTime = nowTime;         // update time of last "second"
  }
}

void screen()
{
  oled.clearDisplay();          // clear display
  oled.setCursor(0,0);          // position cursor
  oled.setTextSize(2);          // text size of 1216 pixels
  oled.print("PPS: ");oled.print(pkt);    // signal (packets) per second
  oled.setCursor(0,16);
  oled.setTextSize(1);          // text size of 68 pixels
  oled.setCursor(0,16);
  oled.print("avg");
  oled.setCursor(40,16);
  oled.print(avgSent,0);        // average of transmitted signals
  oled.setCursor(80,16);
  oled.print(avgPkt,0);         // average of received signals
  oled.setCursor(0,25);
  oled.print("sent");
  oled.setCursor(40,25);
  oled.print(data.sent);        // last number of sent signals
  oled.setCursor(80,25);
  oled.print(data.mins);oled.print(":");    // signal content
  oled.print(data.secs);
  oled.display();
}
