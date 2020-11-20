/*******************************************************************************
 * Sketch name: Transmit joystick signal to control servo motors and laser
 * Description: Transmit numbers as text with RH_ASK library
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    15 - Radio Frequency Communication
 ******************************************************************************/

#include <RH_ASK.h>          // include the RH_ASK library
#include <SPI.h>            // SPI library required to compile
RH_ASK rf;              // associate rf with RH_ASK lib
int timelag = 50;           // interval between transmissions
int switchPin = A0;         // joystick switch pin
int LRpin = A1;           // left-right joystick pin
int FBpin = A2;           // forward-backward joystick pin
int FB, LR, SW;
const char * msg;
String message;

void setup()
{
  rf.init();            // initialise radio transmission
}

void loop()
{     // string for joystick forward-backward, left-right and switch state
  FB = analogRead(FBpin);
  LR = analogRead(LRpin);       // get joystick position
  SW = digitalRead(switchPin);      // and switch state
  message = String(FB) +","+ String(LR) + "," + String(SW) + ",";
  msg = message.c_str();        // convert string
  rf.send((uint8_t *)msg, strlen(msg));   // transmit message
  rf.waitPacketSent();          // wait for transmission to finish
  delay(timelag);           // delay between transmissions
}
