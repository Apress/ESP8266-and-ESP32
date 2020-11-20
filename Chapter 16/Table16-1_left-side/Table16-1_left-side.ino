/*******************************************************************************
 * Sketch name: PWM with ESP8266 microcontroller
 * Description: analogWrite function to control PWM signal
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    16 - Signal generation
 ******************************************************************************/

int wave1Pin = D1, wave2Pin = D2;
int freq1 = 10000, freq2 = 2000;
float duty1, duty2;

void setup()
{
  pinMode(wave1Pin, OUTPUT);
  pinMode(wave2Pin, OUTPUT);
  duty1 = 0.8*1023;
  duty2 = 0.4*1023;
}

void loop()
{
  analogWriteFreq(freq1);
  analogWrite(wave1Pin, duty1);
  analogWriteFreq(freq2);
  analogWrite(wave2Pin, duty2);
}
