/*******************************************************************************
 * Sketch name: Alarm, LED and light intensity
 * Description: email notification of triggered alarm with Cayenne MQTT broker
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    8 - Updating a webpage
 ******************************************************************************/

#include <CayenneMQTTESP8266.h>    // Cayenne MQTT library
char ssid[] = "xxxx";       // change xxxx to your Wi-Fi ssid
char password[] = "xxxx";     // change xxxx to your Wi-Fi password
char username[] = "xxxx";     // change xxxx to Cayenne username
char mqttpass[] = "xxxx";     // change xxxx to Cayenne password
char clientID[] = "xxxx";     // change xxxx to Cayenne client identity
int LEDpin = D3;
int alarmPin = D5;        // define LED, alarm and LDR pins
int LDRpin = A0;
int flashPin = D4;        // flashing LED
int reading, alarm, alert;
int interval = 2000;        // 2s interval between LDR readings
unsigned long LDRtime = 0;

void setup()
{
  Cayenne.begin(username, mqttpass, clientID, ssid, password);
  pinMode(LEDpin, OUTPUT);      // define LED and alarm pins as output
  pinMode(alarmPin, OUTPUT);
  pinMode(flashPin, OUTPUT);
  alarm = 0;          // set alarm to OFF
}

void loop()
{
  Cayenne.loop();         // Cayenne loop function
  if(millis() - LDRtime > interval)
  {
    LDRtime = millis();
    reading = analogRead(LDRpin);
    if (alarm == 1) Cayenne.virtualWrite(V1, reading, "lum", "lux");
    else Cayenne.virtualWrite(V1, 0, "lum", "lux");
    digitalWrite(flashPin, LOW);
    delay(10);          // flash LED to indicate power on
    digitalWrite(flashPin, HIGH);
  }
}

CAYENNE_IN(0)         // Cayenne virtual channel 0
{
  alert = getValue.asInt();     // get alarm triggered status
  digitalWrite(LEDpin, alert);    // update alarm triggered LED
}
CAYENNE_IN(3)         // Cayenne virtual channel 3
{
  alarm = getValue.asInt();     // get alarm set state
  digitalWrite(alarmPin, alarm);    // update alarm set indicator LED
}
