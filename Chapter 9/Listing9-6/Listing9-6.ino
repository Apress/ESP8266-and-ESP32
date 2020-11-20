/*******************************************************************************
 * Sketch name: Real-time sensor graphics
 * Description: update webpage with image and sensor data
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    9 - WebSocket
 ******************************************************************************/

#include <ESP8266WebServer.h>        // include WebServer library
ESP8266WebServer server;        // associate server with library
char ssid[] = "xxxx";         // change xxxx to Wi-Fi SSID
char password[] = "xxxx";       // change xxxx to Wi-Fi password
#include <WebSocketsServer.h>       // include Websocket library
WebSocketsServer websocket = WebSocketsServer(81);  // set WebSocket port 81
#include "buildpage.h"          // webpage AJAX code
String json;
#include <Ticker.h>         // include Ticker library
Ticker timer;           // associate timer with Ticker lib
int interval = 1;
int oldInterval = 1;
#include <Adafruit_Sensor.h>        // include Adafruit Sensor
#include <Adafruit_BMP280.h>        //  and BMP280 libraries
Adafruit_BMP280 bmp;          // associate bmp with BMP280
int BMPaddress = 0x76;          // BMP280 I2C address

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  WiFi.begin(ssid, password);         // connect and initialise Wi-Fi
  while(WiFi.status()!= WL_CONNECTED ) delay(500);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());     // display web server IP address
  server.begin();
  server.on("/",base);          // load default webpage
  server.on("/tempUrl", tempFunct);     // map URL to tempFunct
  websocket.begin();          // initialise WebSocket
  websocket.onEvent(wsEvent);       // wsEvent on WebSocket event
  bmp.begin(BMPaddress);        // initialise BMP280 sensor
  timer.attach(interval, tempFunct);    // attach timer to tempFunct
}
          // function called when message received from client
void wsEvent(uint8_t n, WStype_t type, uint8_t * message, size_t length)
{               // parse message to integer
  if(type == WStype_TEXT) interval = atoi((char *) &message[0]);
}

void tempFunct()      // function to transmit temperature and update interval
{
  JsonConvert(bmp.readTemperature(), interval);   // convert to JSON format
  websocket.broadcastTXT(json.c_str(), json.length());  // send JSON text
  if(interval != oldInterval)
  {
    timer.detach();
    timer.attach(interval, tempFunct);      // update timer interval
    oldInterval = interval;
  }
}
          // function converts variables to JSON name/value pairs
String JsonConvert(float val1, int val2)
{                 // start with open bracket
  json  = "{\"var1\": \"" + String(val1) + "\","; // partition with comma
  json += " \"var2\": \"" + String(val2) + "\"}"; // end with close bracket
  return json;
}
void base()             // function to return HTML code
{
  server.send(200, "text/html", page);
}

void loop()
{
  server.handleClient();        // manage HTTP requests
  websocket.loop();         // handle WebSocket data
}
