/*******************************************************************************
 * Sketch name: OTA updating
 * Description: Update a sketch on a remote microcontroller
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    20 - OTA and saving data to EEPROM, SPIFFS and Excel
 ******************************************************************************/

#include <ArduinoOTA.h>          // include OTA library
#include <ESP8266mDNS.h>        // and mDNS libraries
#include <WiFiUdp.h>          // include Wi-FI UDP library
char ssid[] = "xxxx";         // change xxxx to Wi-Fi ssid
char password[] = "xxxx";       // change xxxx to Wi-Fi password
int LEDpin = 2;           // built-in LED

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  pinMode(LEDpin, OUTPUT);
  WiFi.mode(WIFI_STA);          // initialise Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);           // flash LED while
    flash();            //  connecting to Wi-Fi
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());     // display network port address
  ArduinoOTA.setHostname("WeMos2");     // name network port
  ArduinoOTA.setPassword("admin1");     // set password
  ArduinoOTA.begin();         // initialise ArduinoOTA
}

void loop()
{
  ArduinoOTA.handle();          // check for OTA updates
  digitalWrite(LEDpin, !digitalRead(LEDpin)); // turn on or off LED
  delay(1000);
}

void flash()            // function to flash LED
{
    digitalWrite(LEDpin, HIGH);
    delay(100);
    digitalWrite(LEDpin, LOW);
}
