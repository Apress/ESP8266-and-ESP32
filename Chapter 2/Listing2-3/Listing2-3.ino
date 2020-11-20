/*******************************************************************************
 * Sketch name: Taking a photo and loading to webpage
 * Description: Webpage button to take photo and display on webpage
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    2 - Intranet camera
 ******************************************************************************/
 
#include <esp_camera.h>          // include esp_camera,
#include <ESPAsyncWebServer.h>      // ESPAsyncWebServer libraries
AsyncWebServer server(80);        // associate server with library
#include "config_pins.h"        // configure instructions tab
#include "buildpage.h"          // HTML code for webpage
char ssid[] = "xxxx";         // change xxxx to Wi-Fi ssid
char password[] = "xxxx";       // change xxxx to Wi-Fi password
String pSize;           // photo size (bytes)

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  Serial.println("\n\nsettling down for 2s"); // time to settle vibration
  delay(2000);
  Serial.println("initialising camera, then take photos");
  configCamera();           // function to configure camera
  WiFi.begin(ssid, password);       // initialise Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());     // display WLAN IP address
  server.begin();           // initialise server
  server.on("/",         HTTP_GET, [](AsyncWebServerRequest * request)
  {  request->send_P(200, "text/html", page);});
  server.on("/photoURL", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    camera_fb_t * frame = NULL;
    frame = esp_camera_fb_get();      // take photo as JPEG
    request->send_P(200, "image/jpeg",    // send JPEG image to client
(const uint8_t *)frame->buf, frame->len);
    esp_camera_fb_return(frame);      // clear photo buffer
    pSize = String(frame->len);     // display photo size
    Serial.print("pSize ");Serial.println(pSize);
  });
}

void loop()             // nothing in loop function
{}
