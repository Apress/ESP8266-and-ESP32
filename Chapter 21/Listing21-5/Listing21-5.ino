/*******************************************************************************
 * Sketch name: Analog to digital conversion
 * Description: ADC functionality of ESP32 microcontroller  
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    21 - Microcontrollers
 ******************************************************************************/

int ADCpin = 36;            // define ADC pin

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  Serial.println();
  analogSetPinAttenuation(ADCpin, ADC_11db);  // default setting of 11dB
  Serial.println(analogRead(ADCpin));   // read ADC pin
  analogSetPinAttenuation(ADCpin, ADC_6db); // change setting to 6dB
  Serial.println(analogRead(ADCpin));
  analogSetPinAttenuation(ADCpin, ADC_2_5db);
  Serial.println(analogRead(ADCpin));
  analogSetPinAttenuation(ADCpin, ADC_0db);
  Serial.println(analogRead(ADCpin));
}

void loop()
{}                // nothing in loop function
