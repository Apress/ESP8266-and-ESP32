/*******************************************************************************
 * Sketch name: Receive message with RH_ASK library
 * Description: Receive numbers as text
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    15 - Radio Frequency Communication
 ******************************************************************************/

#include <RH_ASK.h>          // include the RH_ASK library
#include <SPI.h>            // SPI library required to compile
RH_ASK rf (2000, D2, D1, 0);        // associate rf with RH_ASK lib
int LEDpin = D3;            // define LED pin
uint8_t msg[RH_ASK_MAX_MESSAGE_LEN];    // maximum message length
const int nItem = 3;          // number of items in message
String text[nItem];         // define text array
int comma[nItem+1];         // comma positions in message
int LED = 0;            // initial LED state
String message;
float valFlt;
int valInt;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  rf.init();            // initialise radio transmission
  pinMode(LEDpin, OUTPUT);        // define LED pin as output
}

void loop()
{
  uint8_t msglen = sizeof(msg);   // message length based on new message
  if (rf.recv(msg, &msglen))      // message of correct length available
  {
    message = "";         // increment message with each character
    for (int i=0; i<msglen; i++) message = message + char(msg[i]);
    comma[0] = -1;
    for (int i=0; i<nItem; i++)     // comma positions in message
    {               // get substrings between commas
      comma[i+1] = message.indexOf(",", comma[i]+1);
      text[i] = message.substring(comma[i]+1, comma[i+1]);
      Serial.print(text[i]);        // print message substring
      Serial.print(" ");
    }
    valFlt = text[1].toFloat();     // second substring to float
    valInt = text[2].toInt();       // third substring to integer
    Serial.print(text[0]);Serial.print("\t");
    Serial.print(valFlt + 0.05);Serial.print("\t");
    Serial.println(valInt * 2);
    LED = 1 - LED;
    digitalWrite(LEDpin, LED);      // turn on or off LED
  }
}
