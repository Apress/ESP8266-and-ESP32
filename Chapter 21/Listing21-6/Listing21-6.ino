/*******************************************************************************
 * Sketch name: PWM signal
 * Description: PWM functionality of ESP32 microcontroller  
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    21 - Microcontrollers
 ******************************************************************************/

int channel = 0;            // define PWM output channel
int wavePin = 25;           // square wave output pin
int freq = 5000;            // square wave frequency
int resolution = 8;         // PWM resolution
int bright = 0;
int increm = 5;           // increment in duty cycle
int lag = 25;           // time between PWM changes

void setup()
{
  pinMode(wavePin, OUTPUT);       // square wave pin as output
  ledcAttachPin(wavePin, channel);      // attached channel to pin
  ledcSetup(channel, freq, resolution);
}

void loop()
{
  ledcWrite(channel, bright);       // set channel duty cycle
  delay(lag);
  bright = bright + increm;       // increment duty cycle
  if(bright <= 0 || bright >= 255) increm = - increm;
}               // reverse duty cycle increment
