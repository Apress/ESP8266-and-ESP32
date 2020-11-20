/*******************************************************************************
 * Sketch name: PWM with ESP32 microcontroller
 * Description: lcdWrite function to control PWM signal
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    16 - Signal generation
 ******************************************************************************/

int wave1Pin = 25, wave2Pin = 26;
int freq1 = 10000, freq2 = 2000;
float duty1, duty2;
int channel1 = 1, channel2 = 2;
int resolution = 10;

void setup()
{
  pinMode(wave1Pin, OUTPUT);
  pinMode(wave2Pin, OUTPUT);
  ledcAttachPin(wave1Pin, channel1);
  ledcAttachPin(wave2Pin, channel2);
  ledcSetup(channel1, freq1, resolution);
  ledcSetup(channel2, freq2, resolution);
  duty1 = 0.8*1023;
  duty2 = 0.4*1023;
}

void loop()
{
  ledcWrite(channel1, duty1);
  ledcWrite(channel2, duty2);
}
