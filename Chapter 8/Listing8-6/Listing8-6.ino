/*******************************************************************************
 * Sketch name: Parsing data accessed with ThingSpeak API keys
 * Description: Upload environment data from ThingSpeak - combine with Listing 8-7
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    8 - Updating a webpage
 ******************************************************************************/

#include <ESP8266WebServer.h>      // include Webserver library
ESP8266WebServer server;      // associate server with library
WiFiClient client;        // associate client with WiFi library
#include "buildpage.h"        // webpage AJAX code
char ssid[] = "xxxx";       // change xxxx to your Wi-Fi SSID
char password[] = "xxxx";     // change xxxx to your Wi-Fi password
char APItime[] = "xxxx";
char APIdate[] = "xxxx";      // change xxxx to ThingSpeak API key
char APItemp[] = "xxxx";
char APIhumid[] = "xxxx";
char url[] = "/apps/thinghttp/send_request?api_key=";
char host[] = "api.thingspeak.com";
int indexS, indexF, chk, humid;
float temp;
String data, ndata, text, json, mdy, tim;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  WiFi.begin(ssid, password);       // connect and initialise Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());     // display server IP address
  server.begin();           // initialise server
  server.on("/", base);         // load default webpage
  server.on("/API", APIfunct);
}

void APIfunct()
{
  getData(APIdate, "date");       // call function to access date,
  getData(APItime, "time");       //  time,
  getData(APItemp, "temp");       //  temperature,
  getData(APIhumid, "humid");       //  and humidity
  JsonConvert(mdy, tim, temp, humid);   // convert to JSON text
  server.send (200, "text/json", json);
}

String JsonConvert(String val1, String val2, float val3, int val4)
{
  json  = "{\"var1\": \"" + val1         + "\","; // start with {
  json += " \"var2\": \"" + val2         + "\","; // end with comma
  json += " \"var3\": \"" + String(val3) + "\",";
  json += " \"var4\": \"" + String(val4) + "\"}"; // end with }
  return json;
}

void getData(String APIkey, String text)    // function to access data
{
  for (int i=0; i<5; i++)       // with up to five attempts
  {
    getVal(APIkey, text);       // call function to get information
    if(chk > 0) i = 5;          // data accessed successfully
  }
}

void getVal(String APIkey, String text)   // function to access information
{
  chk = 0;
  Serial.print("sourcing ");Serial.println(text);
  client.connect(host, 80);
  client.println(String("GET ") + url + APIkey);
  client.println(String("Host: ") + host);
  client.println("User-Agent: ESP8266/0.1");
  client.println("Connection: close");
  client.println();
  client.flush();
  delay(100);
  while(client.connected())       // while connected to ThingSpeak
  {
    if(client.available())        // if data is available
    {
      data = client.readStringUntil('\n');  // read data till end of line
      Serial.println(data);
      if(text == "humid")       // parse humidity data
      {
        indexS = data.lastIndexOf("=");   // position of last "=" in string
        indexF = data.indexOf("%");
        ndata = data.substring(indexS+2, indexF-2);
        humid = ndata.toInt();
        chk = data.length();
      }
      else if(text == "temp")       // parse temperature data
      {
        indexS = data.indexOf("=");     // position of first "=" in string
        ndata = data.substring(indexS+2);
        temp = ndata.toFloat();
        chk = data.length();
      }
      else if(text == "date")       // date: day month dd, yyyy
      {
        indexS = data.indexOf("is");
        mdy = data.substring(indexS+2);
        chk = data.length();
      }
      else if(text == "time")       // time: hh:mm:ss AM or PM,
      {
        indexF = data.indexOf(",");
        tim = data.substring(0, indexF);
        chk = data.length();
      }
      client.stop();        // close connection after data collected
      delay(100);
    }
  }
}

void base()           // function to return HTML code
{
  server.send (200, "text/html", page);
}

void loop()
{
  server.handleClient();        // handle HTTP requests
}
