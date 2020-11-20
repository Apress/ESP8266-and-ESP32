/*******************************************************************************
 * Sketch name: HTTP request with BMP280 and LED
 * Description: Update webpage with sensor information
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    8 - Updating a webpage
 ******************************************************************************/

#include <ESP8266WebServer.h>        // include ESP8266WebServer lib
ESP8266WebServer server;        // associate server with library
char ssid[] = "xxxx";         // change xxxx to Wi-Fi SSID
char password[] = "xxxx";       // change xxxx to Wi-Fi password
#include <Adafruit_Sensor.h>        // include Unified Sensor
#include <Adafruit_BMP280.h>        //  and BMP280 libraries
Adafruit_BMP280 bmp;          // associate bmp with BMP280 
int BMPaddress = 0x76;          // I2C address of BMP280
#include <Ticker.h>         // include Ticker library
Ticker timer;           // associate timer with Ticker lib
int lag = 10;           // set timer interval at 10s
int LEDpin = D3;            // LED pin on D3
String LED = "off";         // initial LED state
int count = 0;
String temp, counter;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  WiFi.begin(ssid, password);       // initialise Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500); // wait for Wi-Fi connect
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());     // display server IP address
  server.begin();
  server.on("/", webcode);        // map URL to function
  bmp.begin(BMPaddress);        // initialise BMP280
  timer.attach(lag, BMP);       // BMP called every lag seconds
  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, LOW);        // turn off LED
}

void BMP()                // function to get readings
{
  temp = String(bmp.readTemperature());     // update BMP280 reading
  counter = String(count++);          // increment counter
  digitalWrite(LEDpin, !digitalRead(LEDpin));   // turn on or off the LED
  if(LED == "on") LED = "off";        // update LED state
  else LED = "on";
  server.send (200, "text/html", webcode());    // send response to client
}

String webcode()              // return HTML code
{
  String page;
  page = "<!DOCTYPE html><html><head>";
  page += "<meta http-equiv='refresh' content='9'>";    // refresh every 9s
  page += "<title>ESP8266</title></head>";
  page += "<body>";
  page += "<h2>BMP280</h2>";
  page += "<p>Temperature: " + temp + " " + "&degC</p>";  // display temp
  page += "<p>Counter: " + counter + "</p>";      // counter
  page += "<p>LED is " + LED + "<p>";       // LED state
  page += "</body></html>";
  return page;
}

void loop()
{
  server.handleClient();
}
