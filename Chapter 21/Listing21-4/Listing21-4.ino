/*******************************************************************************
 * Sketch name: Interrupts with ESP8266 development board
 * Description: Difference between interrupts on pins D1-D7 or pin D8  
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    21 - Microcontrollers
 ******************************************************************************/

int LEDpin = D0;            // define LED pin
int switchPin = D7;         // define switch pin
volatile int LEDstate = LOW;        // initial LED state
                // volatile as LEDstate in ISR
void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  pinMode(LEDpin, OUTPUT);        // define LEDpin as OUTPUT
  pinMode(switchPin, INPUT_PULLUP);     // when switch pin is D1 to D7
  attachInterrupt(digitalPinToInterrupt(switchPin), change, FALLING);
                // when switch pin is D8
//  attachInterrupt(digitalPinToInterrupt(switchPin), change, RISING);
}

void loop()
{
  Serial.println(digitalRead(switchPin));   // display switch pin state
  delay(1000);
}

IRAM_ATTR void change()         // interrupt service routine (ISR)
{
  LEDstate = 1-LEDstate;        // change LED state
  digitalWrite(LEDpin, LEDstate);     // turn LED on or off
}
