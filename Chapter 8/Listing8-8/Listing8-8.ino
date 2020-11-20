/*******************************************************************************
 * Sketch name: Cayenne, ESP8266 with LED, LDR and BMP820 sensor
 * Description: Display sensor data and control LED on Cayenne dashboard
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    8 - Updating a webpage
 ******************************************************************************/

#include <CayenneMQTTESP8266.h>    // Cayenne MQTT library
char ssid[] = "xxxx";       // change xxxx to Wi-Fi ssid
char password[] = "xxxx";     // change xxxx to Wi-Fi password
char username[] = "xxxx";     // change xxxx to Cayenne username
char mqttpass[] = "xxxx";     // change xxxx to Cayenne password
char clientID[] = "xxxx";     // change xxxx to Cayenne client identity
#include <Adafruit_Sensor.h>      // include Adafruit_Sensor library
#include <Adafruit_BMP280.h>      // include Adafruit_BMP280 library
Adafruit_BMP280 bmp;        // associate bmp with BMP280 library
int LEDpin = D3;          // LED pin
int LDRpin = A0;          // light dependent resistor pin
int flashPin = D4;        // flashing LED pin
unsigned long count = 0;
int interval = 10000;       // 10s interval between MQTT messages
unsigned long lastTime = 0;
float temp, pressure, BasePressure, altitude;
int light;

void setup()
{
  bmp.begin(0x76);        // initiate bmp with I2C address
              // initiate Cayenne MQTT
  Cayenne.begin(username, mqttpass, clientID, ssid, password);
  pinMode(LEDpin, OUTPUT);      // define LED pins as output
  digitalWrite(LEDpin, LOW);
  pinMode(flashPin, OUTPUT);
}

void loop()
{
  Cayenne.loop();           // Cayenne loop function
  if(millis()-lastTime > interval)
  {
    temp = bmp.readTemperature();     // BMP280 temperature
    pressure = bmp.readPressure()/100.0;    //  and pressure
    BasePressure = pressure + 10.0;     // assumed sea level pressure
    altitude = bmp.readAltitude(BasePressure);  // predicted altitude (m)
    light = analogRead(LDRpin);     // ambient light intensity
    light = constrain(light, 0, 1023);    // constrain light reading
    count++;            // increment counter
    if(count>99) count = 0;
    digitalWrite(flashPin, LOW);      // turn on then off flashing LED
    delay(10);
    digitalWrite(flashPin, HIGH);
// send readings to Cayenne on virtual channels 
    Cayenne.virtualWrite(V1, temp, "temp", "c");    // temperature
    Cayenne.virtualWrite(V3, pressure, "bp", "pa");   // pressure
    Cayenne.virtualWrite(V4, altitude, "prox", "m");    // altitude
    Cayenne.virtualWrite(V5, light, "lum", "lux");    // luminosity
    Cayenne.virtualWrite(V6, count,"prox","");      // counter
    lastTime = millis();            // update time
  }
}

CAYENNE_IN(0)           // Cayenne virtual channel 0
{
  digitalWrite(LEDpin, getValue.asInt());   // turn on or off LED
}
