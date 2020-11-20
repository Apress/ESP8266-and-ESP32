/*******************************************************************************
 * Sketch name: ESP-NOW for a receiving ESP8266 microcontroller
 * Description: ESP-NOW receiver with data structure
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    14 - ESP-NOW and LoRa communication
 ******************************************************************************/

#include <espnow.h>          // include ESP-NOW library
typedef struct
{
  int count;            // data structure with
  float value;            // integer, real number
  char text[10];            // and character array
} dataStruct;
dataStruct payload;
int rcv = 0;            // counter of received signals

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  if(esp_now_init() != 0)       // initialise ESP-NOW
  {
    Serial.println("error initialising ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);  // receiver device
  esp_now_register_recv_cb(receiveData);    // call receiveData function
}

void receiveData(uint8_t * mac, uint8_t * data, uint8_t len)
{
  rcv++;              // increment signal counter
  memcpy(&payload, data, sizeof(payload));  // copy received data to payload
  for (int i = 0; i < 6; i++)
  {             // display transmitting MAC address
    Serial.printf("%02x", mac[i]);
    if (i < 5)Serial.print(":");
  }               // display contents of payload
  Serial.print("\t");
  Serial.print("received ");Serial.print(rcv);Serial.print("\t");
  Serial.print("bytes ");Serial.print(len);Serial.print("\t");
  Serial.print("count ");Serial.print(payload.count);Serial.print("\t");
  Serial.print("value ");Serial.print(payload.value);Serial.print("\t");
  Serial.print("text ");Serial.println(payload.text);
}

void loop()             // nothing in loop function
{}
