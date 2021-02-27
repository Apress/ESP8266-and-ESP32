/*******************************************************************************
 * Sketch name: Decoding infrared signals with an ESP32 development board
 * Description: Decode IR signals for controlling MP3 player in Listing 5-4
 * Created on:  February 2021
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    5 - MP3 player
 ******************************************************************************/
#include <IRremote.h>     // include IRremote library version 3.0.1
int IRpin = 23;           // IR receiver pin
String prot[] = {"UNKNOWN","DENON","DISH","JVC","LG","NEC","PANASONIC","KASEIKYO",
                "KASEIKYO_JVC","KASEIKYO_DENON","KASEIKYO_SHARP","KASEIKYO_MITSUBISHI",
                "RC5","RC6","SAMSUNG","SHARP","SONY","APPLE","BOSEWAVE","LEGO_PF",
                "MAGIQUEST","WHYNTER"};   // from irProtocol.h
void setup()
{
  Serial.begin(115200);
  IrReceiver.begin(IRpin);
}

void loop()
{
  if(IrReceiver.decode())
  {
//    IrReceiver.printIRResultShort(&Serial);                  // signal summary
    Serial.print(prot[IrReceiver.decodedIRData.protocol]);   // display protocol,
    Serial.print("\t0x");Serial.print(IrReceiver.decodedIRData.command, HEX);       // signal HEX code
    Serial.print("\tBits ");Serial.println(IrReceiver.decodedIRData.numberOfBits);  //  and bit number
    delay(200);                        // delay before next IR signal
    IrReceiver.resume();               // receive the next value
  }
}
