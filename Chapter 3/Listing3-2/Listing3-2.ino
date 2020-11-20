/*******************************************************************************
 * Sketch name: Pin definitions for ESP8266 and ESP32 development boards
 * Description: Code included at start of sketch for ESP8266 and ESP32
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    3 - International weather station
 ******************************************************************************/

#ifdef ESP32
  int tftCS = 5;            // screen chip select pin
  int tftDC = 26;           // data command select pin
  int tftRST = 25;          // reset pin
#elif ESP8266
  int tftCS = D8;
  int tftDC = D4;
  int tftRST = D0;
#else               // Arduino IDE error message
  #error "ESP8266 or ESP32 microcontroller only"
#endif
