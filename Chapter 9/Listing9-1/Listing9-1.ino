/*******************************************************************************
 * Sketch name: WebSocket main sketch
 * Description: two-way coversation between server and client displayed on webpage
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    9 - WebSocket
 ******************************************************************************/

#include <ESP8266WebServer.h>        // include Webserver library
ESP8266WebServer server;        // associate server with library
#include <WebSocketsServer.h>       // include WebSocket library
WebSocketsServer websocket = WebSocketsServer(81);  // set WebSocket port 81
#include "buildpage.h"          // webpage AJAX code
char ssid[] = "xxxx";         // change xxxx to Wi-Fi SSID
char password[] = "xxxx";       // change xxxx to Wi-Fi password
String str;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  WiFi.begin(ssid, password);       // connect and initialise Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());     // display web server IP address
  server.begin();
  server.on("/", base);         // load default webpage
  websocket.begin();          // initialise WebSocket
  websocket.onEvent(wsEvent);       // call wsEvent function
}               //  on WebSocket event

void wsEvent(uint8_t num, WStype_t type, uint8_t * message, size_t length)
{
  if(type == WStype_TEXT)       // when text received from client
  {               // display text on Serial Monitor
    for(int i=0; i<length; i++) Serial.write(message[i]);
    Serial.println();
  }
}

void loop()
{
  server.handleClient();        // manage HTTP requests
  websocket.loop();         // handle WebSocket data
  if(Serial.available() > 0)
  {               // read text in Serial buffer
    str = Serial.readString();      //  and send to client
    websocket.broadcastTXT(str.c_str(), str.length());
  }
}
