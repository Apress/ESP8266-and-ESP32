/*******************************************************************************
 * Sketch name: Receiver callback
 * Description: ESP-NOW receiver function - see Listing 14-7
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    14 - ESP-NOW and LoRa communication
 ******************************************************************************/

void receiveData(uint8_t * mac, uint8_t * data, uint8_t len)
{
  memcpy(&payload, data, sizeof(payload));  // copy received data to payload
  Serial.print("bytes ");Serial.print(len);Serial.print("\t");
  Serial.print("text ");Serial.println(payload.text);  
}
