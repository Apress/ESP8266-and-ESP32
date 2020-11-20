/*******************************************************************************
 * Sketch name: Receive signal with joystick positions to control servo motors and laser
 * Description: Receive numbers as text with RH_ASK library
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    15 - Radio Frequency Communication
 ******************************************************************************/

#include <RH_ASK.h>          // include the RH_ASK library
#include <SPI.h>            // SPI library required to compile
#include <Servo.h>          // include servo library
RH_ASK rf (2000, D2, D1, 0);        // associate rf with RH_ASK lib
Servo servoFB;            // associate servoFB and servoLR
Servo servoLR;            //  with servo library
int FBpin = D6;           // forward-backward servo pin
int LRpin = D7;           // left-right servo pin
int laserPin = D0;          // define laser pin
uint8_t msg[RH_ASK_MAX_MESSAGE_LEN];
const int nItem = 3;          // number of items in message
String text[nItem];         // array of strings
int comma[nItem+1];         // array of comma positions
String string;
int laser, FB, LR;

void setup()
{
  rf.init();            // initialise radio transmission
  servoFB.attach(FBpin);        // initialise servo motors
  servoLR.attach(LRpin);
  pinMode(laserPin, OUTPUT);        // define laser pin as output
}

void loop()
{
  uint8_t msglen = sizeof(msg); // define message length based on new message
  if (rf.recv(msg, &msglen))    // message of correct length available
  {
    string = "";        // increment string by each message character
    for (int i=0; i<msglen; i++) string = string + char(msg[i]);
    comma[0] = -1;
    for (int i=0; i<nItem; i++)     // get message comma positions
    {               // get substrings between commas
      comma[i+1] = string.indexOf(",", comma[i]+1);
      text[i] = string.substring(comma[i]+1, comma[i+1]);
    }
    FB = map(text[0].toInt(),0,1023,5,100); // map joystick signals to angles
    LR = map(text[1].toInt(),0,1023,5,175);
    laser = text[2].toInt();        // update laser status
    if(laser == HIGH) digitalWrite(laserPin, !digitalRead(laserPin));
    servoFB.write(FB);          // move servos to angles
    servoLR.write(LR);
  }
}
