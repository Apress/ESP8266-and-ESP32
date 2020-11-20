/*******************************************************************************
 * Sketch name: Controlling Pro Micro LEDs
 * Description: Control LEDs directly or with built-in macros 
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    21 - Microcontrollers
 ******************************************************************************/

int RXLED = 17;            // define RXLED pin
//int TXLED = 30;           // required if not using macros

void setup()
{}                // nothing in setup function

void loop()
{
  digitalWrite(RXLED, HIGH);        // turn off RXLED
//  digitalWrite(TXLED, HIGH);      // turn off TXLED
  TXLED0;             // macro to turn off TXLED
  delay(1000);
  digitalWrite(RXLED, LOW);       // turn on RXLED
//  digitalWrite(TXLED, LOW);       // turn on TXLED
  TXLED1;             // macro to turn on TXLED
  delay(1000);
}
