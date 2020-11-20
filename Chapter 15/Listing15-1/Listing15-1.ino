/*******************************************************************************
 * Sketch name: Transmit message with RH_ASK library
 * Description: Transmit numbers as text
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    15 - Radio Frequency Communication
 ******************************************************************************/

#include <RH_ASK.h>          // include the RH_ASK library
#include <SPI.h>            // SPI library required to compile
RH_ASK rf (2000, D2, D1, 0);        // associate rf with RH_ASK lib
int LEDpin = D3;            // define LED pin
String text[] = {"abcdef", "ijkl", "rst"};  // strings of different lengths
const char * msg;           // pointer to array with message
String message;
int timelag = 2000;         // interval between transmissions
int LED = 0;            // initial LED state
int val, len, spd;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  rf.init();            // initialise radio transmission
  pinMode(LEDpin, OUTPUT);        // define LED pin as output
  len = rf.maxMessageLength();      // get maximum message length
  spd = rf.speed();         // get transmission speed
  Serial.print("max message length: ");
  Serial.println(len);          // display message length
  Serial.print("transmission speed: ");   //  and transmission speed
  Serial.println(spd);
}

void loop()
{
  val = millis()/timelag;       // transmission number
  message = text[val%3] + "," + String(1.2*val) + "," + String(val) + ",";
  Serial.println(message);        // display transmitted string
  msg = message.c_str();        // convert string
  rf.send((uint8_t *)msg, strlen(msg));   // transmit signal
  rf.waitPacketSent();          // wait for transmission to finish
  LED = 1 - LED;
  digitalWrite(LEDpin, LED);        // turn on or off LED
  delay(timelag);
}
