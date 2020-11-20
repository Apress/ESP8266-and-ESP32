/*******************************************************************************
 * Sketch name: Bluetooth and LED control sketch for ESP8266 microcontroller
 * Description: Table 10-2 left side
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    10 - Build an app
 ******************************************************************************/

int LEDpin = D4;
int LDRpin = A0;
int bright, LDR;
int LEDstate = 0;
String str;
unsigned int lastTime = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, LEDstate);
}

void loop()
{
  if(Serial.available()>0)
  {
    str = Serial.readString();
    if(str == "C")
    {  
      LEDstate = 1- LEDstate;
      if(LEDstate == 1) Serial.print("H");
      else Serial.print("L");
      digitalWrite(LEDpin, LEDstate);
      delay(20);
    }
    else if(LEDstate == 1)
    {
      bright = str.toInt();
      analogWrite(LEDpin, bright);
    }
  }
  if(millis()-lastTime > 2000)
  {
    lastTime = millis();
    LDR = analogRead(LDRpin);
    Serial.println(LDR);
  }
}
