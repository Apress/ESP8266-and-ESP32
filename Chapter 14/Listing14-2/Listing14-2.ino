/*******************************************************************************
 * Sketch name: Example data structure
 * Description: Data structure with integers, float and text
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    14 - ESP-NOW and LoRa communication
 ******************************************************************************/

typedef struct            // define structure to include
{
  int count = 5;            // two integers,
  int total;
  float value = 3.14;         // a real number
  char text[12] = "text";       // and a character array
} dataStruct;
dataStruct payload;         // name the structure as payload
