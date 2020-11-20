/*******************************************************************************
 * Sketch name: Receiving ESP8266 microcontroller with ESP-NOW and Websocket
 * Description: update webpage with received structure data - see Listing 14-9
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    14 - ESP-NOW and LoRa communication
 ******************************************************************************/

#include <ESP8266WebServer.h>        // include Webserver library
ESP8266WebServer server;
#include <WebSocketsServer.h>       // include WebSocket library
WebSocketsServer websocket = WebSocketsServer(81);  // set WebSocket port 81
#include "buildpage.h"          // webpage AJAX code
char ssid[] = "xxxx";         // change xxxx to Wi-Fi SSID
char password[] = "xxxx";       // change xxxx to Wi-Fi password
#include <espnow.h>         // include ESP-NOW library
typedef struct
{
  int count;            // data structure with
  float value;            // integer, real number
  char text[10];            // and character array
} dataStruct;
dataStruct payload;
String strMAC, message, json;

void setup()
{
  Serial.begin(115200);         // define Serial.Monitor baud rate
  WiFi.begin(ssid, password);       // initialise Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());     // display web server IP address
  Serial.print("MAC address: ");      // and MAC address
  Serial.println(WiFi.macAddress());
  server.begin();
  server.on("/", base);         // default webpage
  websocket.begin();
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
  memcpy(&payload, data, sizeof(payload));  // copy received data to payload
  strMAC = "";
  for (int i = 0; i < 6; i++)       // transmitting MAC address
  {
    strMAC = strMAC + String(mac[i], HEX);  // convert to HEX format
    if (i < 5)strMAC = strMAC + ":";
  }
  strMAC.toUpperCase();         // convert to upper case
  JsonConvert(strMAC, payload.count, payload.value, payload.text);
  websocket.broadcastTXT(json.c_str(), json.length());
}

String JsonConvert(String val1, int val2, float val3, String val4)
{                 // start with open bracket
  json  = "{\"var1\": \"" + val1         + "\","; // partition with comma
  json += " \"var2\": \"" + String(val2) + "\",";
  json += " \"var3\": \"" + String(val3) + "\",";
  json += " \"var4\": \"" + String(val4) + "\"}"; // end with close bracket
  return json;
}

void base()             // function to return HTML code
{
  server.send (200, "text/html", page);
}

void loop()
{
  server.handleClient();        // handle server requests
  websocket.loop();         // handle WebSocket data
}
