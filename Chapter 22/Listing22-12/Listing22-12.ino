/*******************************************************************************
 * Sketch name: Hall effect sensor
 * Description: Detect magnetic field
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    22 - ESP32 microcontroller features
 ******************************************************************************/

int LEDpin = 32;              // define LED pin
unsigned long lastHall = 0;         // time Hall value changed

void setup()
{
  pinMode(LEDpin, OUTPUT);          // LED pin as output
}

void loop()
{
  if(abs(hallRead()) > 30) change();      // call change function
}               // when magnetic field detected

void change()
{
  if(millis() - lastHall < 1000) return;      // check time last change
  lastHall = millis();            // update change time
  digitalWrite(LEDpin, 1 - digitalRead(LEDpin));  // change LED state
}
