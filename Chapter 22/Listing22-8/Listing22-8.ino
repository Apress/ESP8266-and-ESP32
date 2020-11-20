/*******************************************************************************
 * Sketch name: RTC and sleep mode
 * Description: Activate ESP32 microcontroller from sleep mode with switch
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    22 - ESP32 microcontroller features
 ******************************************************************************/

#include <driver/rtc_io.h>        // include rtc input-output library
int switchPin = 32;         // define switch pin
int LEDpin = 26;            // and LED pin
int builtinLED = 2;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  pinMode(LEDpin, OUTPUT);        // LED pins as output
  pinMode(builtinLED, OUTPUT);
  flash();              // call flash function
  rtc_gpio_pullup_en((gpio_num_t)switchPin);    // pull-up switch pin
  esp_sleep_enable_ext0_wakeup((gpio_num_t)switchPin, 0);
}             // wakeup on switch pin with state 0

void loop()
{
  Serial.print("sleep mode on pin ");Serial.println(switchPin);
  esp_deep_sleep_start();         // ESP32 in sleep mode
}

void flash()
{
  for (int i=0; i<3; i++)         // flash LEDs three times
  {
    digitalWrite(LEDpin, HIGH);       // LED on pin
    digitalWrite(builtinLED, HIGH);       // and built-in LED
    delay(200);
    digitalWrite(LEDpin, LOW);
    digitalWrite(builtinLED, LOW);
    delay(100);
  }
}
