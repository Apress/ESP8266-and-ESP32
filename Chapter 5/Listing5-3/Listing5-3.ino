/*******************************************************************************
 * Sketch name: Decoding infrared signals with an ESP32 development board
 * Description: Decode IR signals for controlling MP3 player in Listing 5-4
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    5 - MP3 player
 ******************************************************************************/

#include <IRremote.h>          // include IRremote library version 2.8.0
int IRpin = 23;           // IR receiver pin
IRrecv irrecv(IRpin);
decode_results reading;

void setup()
{
  Serial.begin(115200);
  irrecv.enableIRIn();
}

void loop()
{
  if (irrecv.decode(&reading))
  {
         if(reading.decode_type == NEC)  Serial.print("NEC: ");
    else if(reading.decode_type == SONY) Serial.print("Sony: ");
    else Serial.print("Other: ");
    Serial.print(reading.value, HEX);
    Serial.print("\tBits: ");       // display signal HEX code
    Serial.println(reading.bits);     //  and bit number
    delay(200);           // delay before next IR signal
    irrecv.resume();          // receive the next value
  }
}
