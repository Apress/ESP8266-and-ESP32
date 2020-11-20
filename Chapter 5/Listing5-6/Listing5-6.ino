/*******************************************************************************
 * Sketch name: MP3 player alarm - short version
 * Description: Sound recoding on MP3 player triggered by PIR
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    5 - MP3 player
 ******************************************************************************/

#include<SoftwareSerial.h>        // include SoftwareSerial library
SoftwareSerial SoftSer(D6, D7);     // define SoftSer TX pin
int PIRpin = D2;            // PIR sensor and LED pins
int LEDpin = D3;
byte serialCom[10] = {0x7E,0xFF,0x06,0x12,0x00,0x00,0x03,0xFE,0xE6,0xEF};
                // one control command
void setup()
{
  SoftSer.begin(9600);          // software Serial baud rate
  pinMode(LEDpin, OUTPUT);        // LED pin as OUTPUT
}

void loop()
{
  if(digitalRead(PIRpin) == HIGH)     // PIR sensor triggered
  {
    digitalWrite(LEDpin, HIGH);     // turn on LED and play sound
    for(int i=0; i<10; i++) SoftSer.write(serialCom[i]);
    delay(10000);
    digitalWrite(LEDpin, LOW);      // turn off LED after 10s
  }
}
