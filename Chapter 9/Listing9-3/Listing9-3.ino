/*******************************************************************************
 * Sketch name: Height and distance measure
 * Description: control servo motor and laser on webpage
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    9 - WebSocket
 ******************************************************************************/

#include <ESP8266WebServer.h>        // include Webserver library
ESP8266WebServer server;        // associate server with library
char ssid[] = "xxxx";         // change xxxx to Wi-Fi SSID
char password[] = "xxxx";       // change xxxx to Wi-Fi password
#include <WebSocketsServer.h>       // include Websocket library
WebSocketsServer websocket = WebSocketsServer(81);  // set WebSocket port 81
#include "buildpage.h"          // webpage AJAX code
#include <Servo.h>
Servo servoFB;                     // associate servoFB with servo lib
int FBpin = D7;           // forward-backward servo pin
int laserPin = D8;
int minMicrosec = 450;          // minimum and maximum time
int maxMicrosec = 1150;         // for servo motor pulse length
#include <NewPingESP8266.h>       // include NewPing library
int trigPin = D5;           // ultrasonic trigger and echo pins
int echoPin = D6;
float maxdist = 300;          // ultrasound maximum distance
NewPingESP8266 sonar(trigPin, echoPin, maxdist);
float distance, height, temp, angle;
int microsec, laser, comma;
String text[2];           // strings in JSON text
String str, json;
unsigned long timer = 0;

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
  websocket.onEvent(wsEvent);       // wsEvent on WebSocket event
  servoFB.attach(FBpin);        // initialise servo motor
  servoFB.writeMicroseconds(minMicrosec);   // and move to initial position
  pinMode(laserPin, OUTPUT);        // define laser pin as output
}
          // function called when message received from client
void wsEvent(uint8_t n, WStype_t type, uint8_t * message, size_t length)
{
  if(type == WStype_TEXT)
  {
    str = "";           // convert message to string
    for (int i=0; i<length; i++) str = str + char(message[i]);
    comma = str.indexOf(",");         // location of comma
    text[0] = str.substring(0, comma);      // extract substrings
    text[1] = str.substring(comma+1);
    angle = text[0].toFloat();        // parse servo angle
    microsec = map(angle,20,90,minMicrosec,maxMicrosec);  // map angle to s
    servoFB.writeMicroseconds(microsec);      // move servo to angle
    delay(10);              // time to move servo
    laser = text[1].toInt();          // parse laser state
    digitalWrite(laserPin, laser);        // turn on or off laser
    distance = (sonar.ping_median(10)/2.0)*0.0343;  // get horizontal distance
    angle = angle*PI/180.0;         // convert angle to radians
    height = 5.0+(distance+8.0)*tan(angle)+2.0/cos(angle);  // vertical distance
    JsonConvert(height, distance);        // convert to JSON format
    websocket.broadcastTXT(json.c_str(), json.length());  // send JSON text
  }
}
          // function converts variables to JSON name/value pairs
String JsonConvert(float val1, float val2)
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
  server.handleClient();          // manage HTTP requests
  websocket.loop();           // handle WebSocket data
}
