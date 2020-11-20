/*******************************************************************************
 * Sketch name: Transmitter callback
 * Description: ESP-NOW transmitter function - see Listing 14-6
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    14 - ESP-NOW and LoRa communication
 ******************************************************************************/

void sendData(uint8_t * mac, uint8_t chk)
{
  for (int i=0; i<6; i++)          // receiver MAC address
  {
    Serial.printf("%02x", mac[i]);        // convert to HEX format
    if(i < 5) Serial.print(":");        // include colons
  } 
  Serial.print("\tcallback ");
  if(chk == 0) Serial.println("OK  ");      // transmission received
  else Serial.println("fail");        //  or not
}
