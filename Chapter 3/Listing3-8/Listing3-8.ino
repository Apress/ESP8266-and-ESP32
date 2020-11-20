/*******************************************************************************
 * Sketch name: OpenWeatherMap data example
 * Description: Demonstartion of data sourcing with error checking
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    3 - International weather station
 ******************************************************************************/

#include <ESP8266WiFi.h>      // library to connect to Wi-Fi network
#include <ArduinoJson.h>
WiFiClient client;        // create client to connect to IP address
char ssid[] = "xxxx";       // change xxxx to Wi-Fi SSID
char password[] = "xxxx";     // change xxxx to Wi-Fi password
String APIkey = "xxxx";       // and xxx to openweathermap API key
char server[]="api.openweathermap.org";
String output;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  WiFi.begin(ssid, password);       // initialise Wi-Fi and wait
  while (WiFi.status() != WL_CONNECTED) delay(500); // for Wi-Fi connection
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());     // Wi-Fi network IP address
  Serial.println("Connecting...");
  client.connect(server, 80);       // connect to server on port 80
  if (!client.connect(server, 80))      // connection error message
  {
    Serial.println("Connection failed");
    return;
  }
  Serial.println("Connected!");
  client.println("GET /data/2.5/uvi?lat=55.95&lon=-3.19&"
  "appid="+APIkey+" HTTP/1.1");     // send HTTP request
  client.println("Host: api.openweathermap.org");
  client.println("User-Agent: ESP8266/0.1");
  client.println("Connection: close");
  client.println();
  if (client.println() == 0)        // HTTP request error message
  {
    Serial.println("HTTP request failed");
    return;
  }
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0)
  {               // HTTP status error message
    Serial.print("Response not valid: ");
    Serial.println(status);
    return;
  }
  else Serial.println("HTTP status OK");
  if (!client.find("\r\n\r\n"))     // received data error request
  {
    Serial.println("Received data not complete");
    return;
  }
//  client.find("\r\n\r\n");
  DynamicJsonDocument jsonDoc(1024);
  DeserializationError error = deserializeJson(jsonDoc, client);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");   // JSON error message
    Serial.println(error.c_str());
    return;
  }
  serializeJson(jsonDoc, output);       // display all data
  Serial.print("data length: ");Serial.println(output.length());
  Serial.println(output);
  Serial.println("extracted text");
  Serial.println(jsonDoc["lon"].as<float>(), 2);  // display specific data
  Serial.println(jsonDoc["lat"].as<float>(), 2);
  Serial.println(jsonDoc["date_iso"].as<char*>());
  Serial.println(jsonDoc["date"].as<long>());  
  Serial.print("UV ");Serial.println(jsonDoc["value"].as<float>(), 2);
}

void loop()               // nothing in loop function
{}
