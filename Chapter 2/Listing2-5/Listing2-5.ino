/*******************************************************************************
 * Sketch name: Real-time viewing on webpage
 * Description: Camera images streamed to webpage, Listing 2-6 required as tab
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    2 - Intranet camera
 ******************************************************************************/
 
#include <esp_http_server.h>        // include esp http_server,
#include <esp_camera.h>         // camera and Wi-Fi libraries
#include <WiFi.h>
#include "config_pins.h"        // configure instructions tab
#include "stream_handler.h"       // code to stream images
char ssid[] = "xxxx";         // change xxxx to Wi-Fi ssid
char password[] = "xxxx";       // change xxxx to Wi-Fi password

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  Serial.setDebugOutput(false);     // no debug information
  WiFi.begin(ssid, password);       // initialise Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());     // display WLAN IP address
  configCamera();
  sensor_t * s = esp_camera_sensor_get();   // reduce frame size
  s->set_framesize(s, FRAMESIZE_VGA);   // to 640480 pixels
  startCameraServer();
}

void startCameraServer()      // function to start camera server
{
  httpd_handle_t stream_httpd = NULL;
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;
  httpd_uri_t index_uri = {.uri="/", .method=HTTP_GET, 
                           .handler=stream_handler, .user_ctx=NULL};
  if (httpd_start(&stream_httpd, &config) == ESP_OK)
      httpd_register_uri_handler(stream_httpd, &index_uri);
}

void loop()             // nothing in loop function
{}
