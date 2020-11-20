/*******************************************************************************
 * Sketch name: Capacitive touch sensor
 * Description: Touch sensor as a switch
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    22 - ESP32 microcontroller features
 ******************************************************************************/

int touchPin = T7;            // define touch pin
int LEDpin = 32;              // and LED pin
int threshold = 50;           // limit for touch pin
volatile unsigned long lastTouch = 0;     // time touch pin pressed

void setup()
{
  pinMode(LEDpin, OUTPUT);          // LED pin as output
  touchAttachInterrupt(touchPin, change, threshold);  // define interrupt
}

void change()             // ISR
{
  if (millis() - lastTouch < 1000) return;      // touch pin recently pressed
  lastTouch = millis();           // update touch time
  digitalWrite(LEDpin, 1 - digitalRead(LEDpin));  // change LED state
}

void loop()               // nothing in loop function
{}
