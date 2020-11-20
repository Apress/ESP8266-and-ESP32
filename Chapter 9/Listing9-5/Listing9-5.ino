/*******************************************************************************
 * Sketch name: Servo motor calibration
 * Description: determine square wave pulse length to move servo motor
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    9 - WebSocket
 ******************************************************************************/

#include <Servo.h>        // include Servo library
Servo servoFB;          // associate servoFB with servo library
int FBpin = D7;         // servo pin
int microsec;

void setup()
{
  Serial.begin(115200);       // define Serial Monitor baud rate
  servoFB.attach(FBpin);      // initialise servo motor
}

void loop()
{
  if(Serial.available() > 0)      // text entered in Serial Monitor
  {
    microsec = Serial.parseInt();   // parse text to integer
    servoFB.writeMicroseconds(microsec);  // move servo motor
  }
}
