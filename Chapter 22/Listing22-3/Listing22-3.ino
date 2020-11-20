/*******************************************************************************
 * Sketch name: Bluetooth communication
 * Description: Transmit and receive messages with Bluetooth
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    22 - ESP32 microcontroller features
 ******************************************************************************/

#include <BluetoothSerial.h>        // include Bluetooth library
BluetoothSerial SerialBT;       // associate SerialBT with library
String str;
int strLen;
char c;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  SerialBT.begin("ESP32 Bluetooth");    // identify Bluetooth device
}

void loop()
{           // received message from Bluetooth device
  if(SerialBT.available()) Serial.write(SerialBT.read());
  if(Serial.available())          // message to transmit
  {
    str = Serial.readString();        // read and display
    Serial.print("\t\t\t\t");Serial.println(str); //  Serial buffer
    strLen = str.length();
    for (int i=0; i<strLen; i++)
    {
      c = str[i];           // for each message character
      SerialBT.write(c);        // transmit to Bluetooth device
    }
    SerialBT.write('\n');       // add new line character
  }
  delay(50);
}
