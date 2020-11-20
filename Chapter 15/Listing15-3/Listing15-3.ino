/*******************************************************************************
 * Sketch name: Receive and decode an RF code with rc-switch library
 * Description: Receive RF code
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    15 - Radio Frequency Communication
 ******************************************************************************/

#include <RCSwitch.h>          // include the rc-switch library
RCSwitch rc = RCSwitch();       // associate rc with rc-switch lib
int LEDpin = D3;            // define LED pin and state
int LED = 0;
unsigned long value;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  digitalPinToInterrupt(D2);        // set pin as interrupt
  rc.enableReceive(D2);         // receive data on interrupt pin
  pinMode(LEDpin, OUTPUT);        // define LED pin as output
}

void loop()
{
  if (rc.available())           // if a signal is received
  {
    value = rc.getReceivedValue();        // signal in decimal format
    if (value != 0)           // non-zero signal value
    {
      Serial.print("Decimal ");Serial.print(value);
      Serial.print(" (");
      Serial.print(rc.getReceivedBitlength());    // signal bit length
      Serial.print("bit)\tProtocol ");      // print a tab between text
      Serial.println(rc.getReceivedProtocol());   // signal protocol class
      LED = 1 - LED;
      digitalWrite(LEDpin, LED);        // turn on or off the LED
    }
    else Serial.println("Unknown encoding");
    rc.resetAvailable();          // ready to receive signal
  }
}
