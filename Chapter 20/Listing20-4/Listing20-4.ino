/*******************************************************************************
 * Sketch name: List directory files with SPIFFS for ESP32 microcontroller
 * Description: Replace ESP8266 function in Listing 20-3
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    20 - OTA and saving data to EEPROM, SPIFFS and Excel
 ******************************************************************************/

void dirContent(String dname)
{
  Serial.print(dname);Serial.println(" content");
  File dir = SPIFFS.open(dname);          // SPIFFS library
  File file = dir.openNextFile();         // openNextFile
  while(file)
  {
    Serial.print("file ");Serial.print(file.name());Serial.print("\t");
    Serial.print("size ");Serial.println(file.size());
    file = dir.openNextFile();          // openNextFile
  }
}
