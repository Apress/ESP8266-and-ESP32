/*******************************************************************************
 * Sketch name: Transmit an RF code with rc-switch library
 * Description: Transmit RF code
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    15 - Radio Frequency Communication
 ******************************************************************************/

#include <RCSwitch.h>          // include the rc-switch library
RCSwitch rc = RCSwitch();       // associate rc with rc-switch lib
unsigned long value = 3163905;      // code to be transmitted
const char * biCode;          // pointers to arrays with number
const char * triCode;         // in binary or Tri-State format
int LEDpin = D3;            // define LED pin
int LED = 0;            // initial LED state
int delTime = 1000;         // delay between transmissions
String bin, tri;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  pinMode(LEDpin, OUTPUT);        // define LED pin as output
  rc.enableTransmit(D1);        // transmit data pin
  rc.setPulseLength(351);       // optional with time in ms
  rc.setProtocol(1);          // default is 1
  rc.setRepeatTransmit(4);        // define number of transmissions
 }

void loop()
{ 
  rc.send(value, 24);         // send number in decimal format
  Serial.print(value);Serial.print("\t");   // display value
  digitalWrite(LEDpin, LED);        // turn on or off LED
  LED = 1 - LED;
  delay(delTime);           // interval between transmissions
  binary(value);            // convert to binary format
  rc.send(biCode);          // send array in binary format
  Serial.print(biCode);Serial.print("\t");  // display value in binary format
  delay(delTime);
  tristate(bin);            // convert to Tri-State format
  rc.sendTriState(triCode);       // send array in Tri-State format
  Serial.println(triCode);        // display in Tri-code format
  delay(delTime);
  value++;
  if(value > 3163913) for (;;) delay(1000); // stop after sending all codes
}

void binary(long number)      // function to convert to binary format
{
  bin = "";
  for (int i=0; i<24; i++)      // 24 bits starting with LSB
  {             // next bit precedes lower significant bits
    if(number%2 == 1) bin = "1" + bin;  // number is an unsigned long integer
    else bin = "0" + bin;
    number = number/2;
  }
  biCode = bin.c_str();       // create pointer to array
}

void tristate(String val)       // function to convert to Tri-State format
{
  tri = "";
  for (int i=0; i<12; i++)      // start with MSB which is charAt(0)
  {             // next bit follows higher significant bits
        if(val.charAt(2*i)=='0' && val.charAt(2*i+1)=='0') tri = tri + "0";
   else if(val.charAt(2*i)=='1' && val.charAt(2*i+1)=='1') tri = tri + "1";
   else if(val.charAt(2*i)=='0' && val.charAt(2*i+1)=='1') tri = tri + "F";
   else if(val.charAt(2*i)=='1' && val.charAt(2*i+1)=='0') tri = tri + "U";
  }
  triCode = tri.c_str();      // create pointer to array
}
