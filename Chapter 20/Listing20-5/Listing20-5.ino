/*******************************************************************************
 * Sketch name: Downloading SPIFFS data file
 * Description: User determines the directory to sore the SPIFFS file
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    20 - OTA and saving data to EEPROM, SPIFFS and Excel
 ******************************************************************************/

#include <LittleFS.h>          // include LittleFS and
#include <ESPAsyncWebServer.h>      // ESPAsyncWebServer libraries
AsyncWebServer server(80);
char ssid[] = "xxxx";         // change xxxx to Wi-Fi ssid
char password[] = "xxxx";       // change xxxx to Wi-Fi password
String filename;            // file to be downloaded

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  WiFi.begin(ssid, password);       // initialise Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());     // display WLAN IP address
  server.begin();           // initialise server
  server.on("/download", HTTP_GET, [](AsyncWebServerRequest * request)
  {         request->send(LittleFS, filename, "text/plain", true); });
  LittleFS.begin();         // initialise SPIFFS
  dirContent("");           // contents of main directory
  dirContent("temp");         // content of "temp" sub-directory
  Serial.println("\nEnter directory/filename to download");
}

void loop()
{
  if(Serial.available() > 0)      // filename entered on Serial Monitor
  {
    filename = Serial.readString();     // read Serial buffer
    Serial.print("In the browser, enter ");Serial.print(WiFi.localIP());
    Serial.print("/download to download file: ");Serial.println(filename);
    Serial.println("\n\nEnter directory/filename to download");
  }
}

void dirContent(String dname)     // function to display directory content
{
  Serial.print("\nContent of directory: ");Serial.println(dname);
  Dir dir = LittleFS.openDir("/"+dname);
  while(dir.next())
  {
    File file = dir.openFile("r");          // read file
    Serial.print(dname);Serial.print("/");
    Serial.print(file.name());Serial.print("\t");
    Serial.print("size ");Serial.println(file.size());  // filesize
  }
}
