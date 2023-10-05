/*******************************************************************************
 * Sketch name: Blynk, ESP8266 with LED, LDR and BMP820 sensor
 * Description: Display sensor data and control LED on Blynk dashboard
 * Created on:  October 2023
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    8 - Updating a webpage
 ******************************************************************************/

#define BLYNK_TEMPLATE_ID   "xxxx"    // change xxxx to Template details
#define BLYNK_TEMPLATE_NAME "Listing88"
#define BLYNK_AUTH_TOKEN    "xxxx"    // change xxxx to Authentication token
#define BLYNK_PRINT Serial
//#include <ESP8266WiFi.h>  // called by BlynkSimpleEsp8266
#include <BlynkSimpleEsp8266.h>
#include <ssid_password.h>
#include <Adafruit_Sensor.h>      // include Adafruit_Sensor library
#include <Adafruit_BMP280.h>      // include Adafruit_BMP280 library
Adafruit_BMP280 bmp;        // associate bmp with BMP280 library
int LEDpin = D3;          // LED pin
int LDRpin = A0;          // light dependent resistor pin
int flashPin = D4;        // flashing LED pin
int count = 0;
int interval = 10000;       // 10s interval between MQTT messages
float temp, pressure;
int light;

BlynkTimer timer;
void timerEvent()
{
  temp = bmp.readTemperature();     // BMP280 temperature
  pressure = bmp.readPressure()/100.0;    //  and pressure
  light = analogRead(LDRpin);     // ambient light intensity
  light = constrain(light, 0, 1023);    // constrain light reading
  count++;            // increment counter
  if(count>99) count = 0;
  digitalWrite(flashPin, LOW);      // turn on then off flashing LED
  delay(10);
  digitalWrite(flashPin, HIGH);     // send readings to Blynk on virtual pins 
  Blynk.virtualWrite(V1, temp);    // temperature
  Blynk.virtualWrite(V3, pressure);   // pressure
  Blynk.virtualWrite(V5, light);    // luminosity
  Blynk.virtualWrite(V6, count);      // counter
}

BLYNK_WRITE(V0)           // Blynk virtual pin 0
{
  digitalWrite(LEDpin, param.asInt());   // turn on or off LED
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
  timer.setInterval(interval, timerEvent);
  bmp.begin(0x76);        // initiate bmp with I2C address
  pinMode(LEDpin, OUTPUT);      // define LED pins as output
  digitalWrite(LEDpin, LOW);
  pinMode(flashPin, OUTPUT);
}

void loop()
{
  Blynk.run();           // 
  timer.run();
}
