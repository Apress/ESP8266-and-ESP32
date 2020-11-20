/*******************************************************************************
 * Sketch name: Receive signal to control load
 * Description: Receive RC codes with rc-switch library
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    15 - Radio Frequency Communication
 ******************************************************************************/

#include <RCSwitch.h>          // include the rc-switch library
RCSwitch rc = RCSwitch();       // associate rc with rc-switch lib
int LEDpin = D3;            // LED to change state
int LEDrelayPin = D0;         // LED associated with relay
int relayPin = D7;          // define MOSFET/relay pin
unsigned long value;

void setup()
{
  Serial.begin(9600);         // define Serial Monitor baud rate
  digitalPinToInterrupt(D2);        // set pin as interrupt
  rc.enableReceive(D2);         // receive data on interrupt pin
//  digitalWrite(relayPin, HIGH);     // set relayPin HIGH before 
  pinMode(relayPin, OUTPUT);        //  defining relayPin
  pinMode(LEDpin, OUTPUT);
  pinMode(LEDrelayPin, OUTPUT);
}

void loop()
{
  if (rc.available())         // if a signal is received
  {
    value = rc.getReceivedValue();
    if (value != 0)         // signal value not equal to zero
    {               // display signal value
      Serial.print("code ");Serial.print(value);
      if(value == 3163908)        // Light button pressed
      {
        Serial.print("\tchange LED");   // display action
        digitalWrite(LEDpin, !digitalRead(LEDpin)); // turn on or off LED
      }
      else if(value == 3163909)     // Bright+ button pressed
      {
        Serial.print("\trelay on");
        digitalWrite(LEDrelayPin, HIGH);    // turn on relay LED
        digitalWrite(relayPin, HIGH);   // turn on relay
      }
      else if (value == 3163910)      // Bright- button pressed
      {
        Serial.print("\trelay off");
        digitalWrite(LEDrelayPin, LOW);   // turn off relay LED
        digitalWrite(relayPin, LOW);    // turn off relay
      }
      else Serial.print("\tno action");
      Serial.print("\tLED   ");
      Serial.print(digitalRead(LEDpin));    // display LED and relay states
      Serial.print("\trelay ");
      Serial.println(digitalRead(relayPin));
    }
    else Serial.println("Unknown encoding");
    rc.resetAvailable();        // ready to receive new signal
  }
}
