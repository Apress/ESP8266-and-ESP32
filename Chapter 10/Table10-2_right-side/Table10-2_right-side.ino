/*******************************************************************************
 * Sketch name: Bluetooth and LED control sketch for ESP32 microcontroller
 * Description: Table 10-2 right side
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    10 - Build an app
 ******************************************************************************/

#include <BluetoothSerial.h>
BluetoothSerial SerialBT;
int LEDpin = 27;
int LDRpin = 33;
int bright, LDR;
int LEDstate = 0;
int channel = 0;
char c;
String str;
unsigned int lastTime = 0;

void setup()
{
  SerialBT.begin("ESP32 Bluetooth");
  pinMode(LEDpin, OUTPUT);
  ledcAttachPin(LEDpin, channel);
  ledcSetup(channel, 1000, 8);
  ledcWrite(channel, LEDstate);
}

void loop()
{
  if(SerialBT.available()>0)
  {
    str = "";
    while(SerialBT.available()>0)
    {
      c = SerialBT.read();
      str = str + String(c);
    }
    if(str == "C" )
    {
      LEDstate = 1 - LEDstate;
      if(LEDstate == 1) SerialBT.print("H");
      else SerialBT.print("L");
      ledcWrite(channel, LEDstate*255);
      delay(20);
    }
    else if(LEDstate == 1)
    {
      bright = str.toInt(); 
      ledcWrite(channel, bright);
    }
  }
  if(millis()-lastTime > 2000)
  {
    lastTime = millis();
    LDR = analogRead(LDRpin);
    SerialBT.println(LDR);
  }
}
