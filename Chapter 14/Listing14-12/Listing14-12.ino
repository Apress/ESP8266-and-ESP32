/*******************************************************************************
 * Sketch name: Receiving LoRa module and Websocket
 * Description: update webpage with received message - see Listing 14-13
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    14 - ESP-NOW and LoRa communication
 ******************************************************************************/

#include <ESP8266WebServer.h>        // include Webserver library
ESP8266WebServer server;        // associate server with library
#include <WebSocketsServer.h>       // include WebSocket library
WebSocketsServer websocket = WebSocketsServer(81);  // set WebSocket port 81
#include "buildpage.h"          // webpage AJAX code
char ssid[] = "xxxx";         // change xxxx to Wi-Fi SSID
char password[] = "xxxx";       // change xxxx to Wi-Fi password
String message, json;
int RSSI;
float SNR;
#include <SPI.h>            // include SPI library
#include <LoRa.h>           //  and LoRa library
int CSS = D8;           // define SX1278 pins
int RST = -1;           // RESET pin
int DIO0 = D2;            // interrupt pin
int packetSize;

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
  digitalPinToInterrupt(DIO0);      // set pin as interrupt
  LoRa.setPins(CSS, RST, DIO0);     // define LoRa module pins
  while (!LoRa.begin(433E6)) delay(500);    // 433MHz transmission
  Serial.println("LoRa connected");
}

void loop()
{
  server.handleClient();          // handle HTTP requests
  websocket.loop();           // handle WebSocket data
  packetSize = LoRa.parsePacket();        // detect received packet
  if (packetSize > 0)
  {
    message = "";             // read packet
    while(LoRa.available()) message = message + ((char)(LoRa.read()));
    RSSI = LoRa.packetRssi();
    SNR = LoRa.packetSnr();         // signal : noise ratio
    JsonConvert(message, RSSI, SNR);      // convert to JSON format
    websocket.broadcastTXT(json.c_str(), json.length());
  }
}

String JsonConvert(String val1, int val2, float val3)
{                 // start with open bracket
  json  = "{\"var1\": \"" + String(val1) + "\","; // partition with comma
  json += " \"var2\": \"" + String(val2) + "\",";
  json += " \"var3\": \"" + String(val3) + "\"}"; // end with close bracket
  return json;
}

void base()             // function to return HTML code
{
  server.send (200, "text/html", page);
}
