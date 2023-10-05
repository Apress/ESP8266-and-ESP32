/*******************************************************************************
 * Sketch name: Alarm, LED and light intensity
 * Description: email notification of triggered alarm with Blynk MQTT broker
 * Created on:  October 2023
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    8 - Updating a webpage
 ******************************************************************************/

#define BLYNK_TEMPLATE_ID   "xxxx"    // change xxxx to Template details
#define BLYNK_TEMPLATE_NAME "Listing89"
#define BLYNK_AUTH_TOKEN    "xxxx"    // change xxxx to Authentication token
#define BLYNK_PRINT Serial
//#include <ESP8266WiFi.h>  // called by BlynkSimpleEsp8266
#include <BlynkSimpleEsp8266.h>
#include <ssid_password.h>
int LEDpin = D3;
int alarmPin = D5;        // define LED, alarm and LDR pins
int LDRpin = A0;
int flashPin = D4;        // flashing LED
int reading, alarm, alert;
unsigned long lag = 0;
int flag;

BlynkTimer timer;
void timerEvent()
{
  reading = analogRead(LDRpin);
  if (alarm == 1) Blynk.virtualWrite(V1, reading);
  else Blynk.virtualWrite(V1, 0);
  digitalWrite(flashPin, LOW);
  delay(10);          // flash LED to indicate power on
  digitalWrite(flashPin, HIGH);
  
  if(alarm == 1 && reading > 400 && flag == 0)
  {
    Blynk.virtualWrite(V0, 1);
    Blynk.logEvent("alert", "light value above 400: " + String(reading));
    digitalWrite(LEDpin, alarm);
    flag = 1;
  }
  else if (reading < 400 && flag == 1) flag = 0;
}

BLYNK_WRITE(0)         // Blynk virtual pin 0
{
  alert = param.asInt();     // get alarm triggered status
  digitalWrite(LEDpin, alert);    // update alarm triggered LED
}
BLYNK_WRITE(3)         // Blynk virtual pin 3
{
  alarm = param.asInt();     // get alarm set state
  digitalWrite(alarmPin, alarm);    // update alarm set indicator LED
}

BLYNK_CONNECTED()
{
  Serial.println("ESP8266 connected to Blynk");
}

void setup()
{
  Serial.begin(115200);
  Serial.println();Serial.println();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssidEXT, password, "blynk.cloud", 80);
  timer.setInterval(2000, timerEvent);
  pinMode(LEDpin, OUTPUT);      // define LED and alarm pins as output
  pinMode(alarmPin, OUTPUT);
  pinMode(flashPin, OUTPUT);
  alarm = 0;          // set alarm to OFF
}

void loop()
{
  Blynk.run();           // 
  timer.run();
}
