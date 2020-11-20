/*******************************************************************************
 * Sketch name: Decoding infrared signals with an ESP8266 development board
 * Description: Decode IR signals to control MP3 player in Listing 3-5
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    5 - MP3 player
 ******************************************************************************/

#include <IRutils.h>          // include IRutils library
int IRpin = D1;           // IR receiver pin
int BufferSize = 1024;          // longer signal length
int Timeout = 50;           // block repeat signals
IRrecv irrecv(IRpin, BufferSize, Timeout, true);
decode_results reading;         // IRremote reading

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  irrecv.enableIRIn();          // initialise the IR receiver
}

void loop()
{
  if (irrecv.decode(&reading))      // read pulsed signal
     Serial.print(resultToHumanReadableBasic(&reading));
}               // display signal information
