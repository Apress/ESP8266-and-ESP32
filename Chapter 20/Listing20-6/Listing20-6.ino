/*******************************************************************************
 * Sketch name: Saving data directly to an Excel file
 * Description: MUST BE ABLE TO STOP KEYBOARD
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    20 - OTA and saving data to EEPROM, SPIFFS and Excel
 ******************************************************************************/

#include <Keyboard.h>          // include Keyboard library
#include <Adafruit_Sensor.h>        // include Unified Sensor library
#include <Adafruit_BMP280.h>        // include BMP280 library
Adafruit_BMP280 bmp;          // associate bmp with BMP280
int BMPaddress = 0x76;          // I2C address of BMP280
int switchPin = A3;         // define switch and LDR pins
int LDRpin = 9;
unsigned long nowTime, lastTime = 0;
float temp, bright;
int counter = 0;

void setup()
{
  Keyboard.begin();         // initialise Keyboard
  while(!Keyboard);         // delay to setup Keyboard
  bmp.begin(BMPaddress);        // initialise BMP280
  header();             // call header function
}

void loop()
{           // MUST BE ABLE TO STOP KEYBOARD
  if(digitalRead(switchPin) == HIGH)    // switch to stop Keyboard
  {
    Keyboard.end();         // stop Keyboard
    while(1);           // and do nothing else
  }
  nowTime = millis();         // set start of time interval
  if(nowTime - lastTime > 5000)     // collect data every 5s
  {
    counter++;            // increment counter
    temp = bmp.readTemperature();     // get BMP280 reading
    bright = analogRead(LDRpin);      // and brightness reading
    Keyboard.print(counter);        // print counter to Excel
    Keyboard.print(char(9));        // print tab character
    Keyboard.print(temp);       // print temp to Excel
    Keyboard.print(char(9));        // print bright to Excel, with
    Keyboard.println(bright);       // carriage return and new line
    lastTime = nowTime;         // update start of time interval
  }
}

void header()       // function to print columns headers to Excel
{
  Keyboard.print("counter");        // print "counter" to Excel
  Keyboard.print(char(9));        // print tab character
  Keyboard.print("temp");
  Keyboard.print(char(9));
  Keyboard.print("bright");
  Keyboard.print(char(13));       // print carriage return character
  Keyboard.print(char(10));       // print new line character
}
