/*******************************************************************************
 * Sketch name: ESP-NOW for a transmitting ESP8266 microcontroller
 * Description: ESP-NOW transmitter with data structure
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    14 - ESP-NOW and LoRa communication
 ******************************************************************************/

#include <espnow.h>          // include ESP-NOW library
uint8_t receiveMAC[] = {0x84,0xF3,0xEB,0x0D,0xB5,0xB3};
typedef struct            // receiver MAC address
{
  int count = 0;            // data structure with
  float value = 3.14;         // integer, real number
  char text[10] = "abcdef";       // and character array
} dataStruct;
dataStruct payload;
int channel = 1;            // set transmission channel
int chk;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  if(esp_now_init() != 0)       // initialise ESP-NOW
  {
    Serial.println("error initialising ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER); // transmitter device
  chk = esp_now_add_peer(receiveMAC, ESP_NOW_ROLE_SLAVE, channel, NULL, 0);
  if(chk == 0) Serial.println("receiver added");  // add receiver device
  else
  {
    Serial.println("error adding receiver");
    return;
  }
  esp_now_register_send_cb(sendData);     // call sendData function
}

void loop()
{
  payload.count++;            // increment counter
  payload.value = payload.value + 1.0;      // and real number
  if(strcmp(payload.text,"abcdef") == 0)      // alternate text
strncpy(payload.text, "xyz", sizeof(payload.text));
  else strcpy(payload.text, "abcdef");
  Serial.print(payload.count);
  Serial.print(payload.value);        // display transmitted data
  Serial.print(payload.text);
  chk = esp_now_send(receiveMAC, (uint8_t *) & payload, sizeof(payload));
  Serial.print("\tsent ");
  if(chk == 0) Serial.print("OK  ");      // transmission sent or not
  else Serial.println("fail");
  delay(2000);
}

void sendData(uint8_t * mac, uint8_t chk)     // callback function
{
  for (int i = 0; i < 6; i++)       // display receiving MAC address
  {
    Serial.printf("%02x", mac[i]);
    if (i < 5)Serial.print(":");
  } 
  Serial.print("\tcallback ");      // transmission received or not
  if(chk == 0) Serial.println("OK  ");
  else Serial.println("fail");
}
