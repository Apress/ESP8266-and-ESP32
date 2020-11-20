/*******************************************************************************
 * Sketch name: OpenWeathermap information
 * Description: Comprehensive weather data displayed on two screens
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    3 - International weather station
 ******************************************************************************/

#include <ESP8266WiFi.h>        // include ESP8266WiFi library
#include <ArduinoJson.h>        // include JSON library
#include <TimeLib.h>          // include TimeLib library
#include <TFT_eSPI.h>         // include TFT_eSPI library
TFT_eSPI tft = TFT_eSPI();        // associate tft with TFT-eSPI lib
uint16_t calData[] = {450, 3400, 390, 3320, 3}; // calibration parameters
uint16_t x = 0, y = 0;
WiFiClient client;        // create client to connect to IP address
char ssid[] = "xxxx";       // change xxxx to Wi-Fi SSID
char password[] = "xxxx";     // change xxxx to Wi-Fi password
String APIkey = "xxxx";       // change xxxx to weathermap API key
String city[] = {"ED","GZ","BR","UA"};    // Edinburgh, Günzburg, Brisbane, Ushuaia
// openweathermap city identity codes
String cityID[] = {"2650225","2913555","2174003","3833367"};
int Ncity = 4;            // number of cities
int cityNow = 0;            // current city, initially set at 0
int count = 99;           // run getWeather function at start
char server[] = "api.openweathermap.org";
int screenFlag = 0;         // flag for first or second screen
int touchFlag = 0;          //  to indicate screen pressed
String days[] = {"",
"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
String mths[] = {"",
"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
String wkdy, mth, addb, text;
int sunriseh, sunrisem, sunseth, sunsetm, dy, hr, mn;
unsigned int chkTime;
DynamicJsonDocument jsonDoc(1024);      // JSON formatted data

void setup()
{
  WiFi.begin(ssid, password);       // initialise Wi-Fi and wait
  while (WiFi.status() != WL_CONNECTED) delay(500); // for WiFi connection
  tft.init();           // initialise screen for graphics
  tft.setTouch(calData);        // calibration parameters
  tft.setRotation(2);         // portrait, connections on top
  tft.fillScreen(TFT_BLACK);        // fill screen in black
  tft.drawRect(0,0,239,319,TFT_WHITE);    // draw white frame line
  tft.drawRect(1,1,237,317,TFT_WHITE);    //  and second frame
}

void loop()
{
  checkTime();            // check if time to refresh screen
  if(tft.getTouch(&x, &y)>0 && touchFlag == 0)  // if screen pressed
  {
    touchFlag = 1;
    if(y > 290)           // city to be selected
    {
      screenFlag = 0;         // start with first screen 
           if(x > 215)          cityNow = 0;
      else if(x > 195 && x<216) cityNow = 1;
      else if(x > 175 && x<196) cityNow = 2;  // select city
      else if(x > 155 && x<176) cityNow = 3;
    }
    if(y < 20) screenFlag = 1-screenFlag;   // change screen
    count = 99;         // run getWeather function immediately
  }
}

void checkTime()            // check for screen refresh
{
  if(count > 4)           // update screen every 5 minutes
  {
    getWeather();           // call weather report function
    count = 0;            // reset counter
  }
  else if(millis()-chkTime>60000)     // increment counter after 60s
  {
    chkTime = millis();         // reset timer
    count++;            // increment counter
  }
}

void getWeather()           // function to get weather data
{
  connect();            // call Wi-Fi connection function
  if(screenFlag == 0) firstScreen();    // select screen to be displayed
  else secondScreen();
  touchFlag = 0;            // reset touch indicator
}

void connect()            // function for Wi-Fi connection
{
  client.connect(server, 80);       // connect to server on port 80
  client.println("GET /data/2.5/weather?id="+cityID[cityNow]+
  "&units=metric&appid="+APIkey+" HTTP/1.1"); // send HTTP request
  client.println("Host: api.openweathermap.org");
  client.println("User-Agent: ESP8266/0.1");
  client.println("Connection: close");
  client.println();
  client.find("\r\n\r\n");        // essential instruction
  DeserializationError error = deserializeJson(jsonDoc, client);
}

void firstScreen()          //weather data for first screen
{
  citySquare();           // call function to display header
  String weather = jsonDoc["weather"][0]["main"];
  String weather2 = jsonDoc["weather"][1]["main"];
  String id1 = jsonDoc["weather"][1]["id"];
  float temp = jsonDoc["main"]["temp"];
  float pres = jsonDoc["main"]["pressure"]; // convert JavaScript objects
  float humid = jsonDoc["main"]["humidity"];  // to strings or real numbers
  float tempMin = jsonDoc["main"]["temp_min"];
  float tempMax = jsonDoc["main"]["temp_max"];
  if(id1.length()<1) weather2 = " ";
  screen("Temperature",TFT_GREEN,5,55,3);   // display weather variable name
  text = String(temp,1);        // convert value to string
  if(temp<9.95) text = " "+text;      // add space if less than 10
  screen(text,TFT_WHITE,45,85,4);     // display string on screen
  screen("o", TFT_WHITE,148,80,3);      // add °symbol
  screen("C", TFT_WHITE,170,85,4);      // add C for Celsius
  screen("min",TFT_BLUE,37,120,2);      // minimum temperature
  text = String(tempMin,1);
  if(tempMin<10) text = " "+text;
  screen(text,TFT_WHITE,20,145,2);
  screen("o",TFT_WHITE,70,135,2);
  screen("C",TFT_WHITE,85,145,2);
  screen("max",TFT_RED,163,120,2);      // maximum temperature
  text = String(tempMax,1);
  if(tempMax<10) text = " "+text;
  screen(text,TFT_WHITE,145,145,2);
  screen("o",TFT_WHITE,197,135,2);
  screen("C",TFT_WHITE,212,145,2);
  screen("Forecast",TFT_GREEN,50,175,3);    // forecast weather
  addb = blank(weather);        // add spaces after text
  weather=weather+addb;
  screen(weather,TFT_WHITE,20,205,3);
  if(weather2 == "null") weather2="";
  addb = blank(weather2);
  weather2=weather2+addb;
  screen(weather2,TFT_WHITE,20,235,3);    // forecast weather detail
  screen("humidity",TFT_GREEN,20,270,2);    // humidity
  text = String(humid,0)+"% ";
  screen(text,TFT_WHITE,40,295,2);
  screen("pressure",TFT_GREEN,130,270,2);   // pressure
  text = String(pres,0);
  if(pres<1000) text = " "+text;
  screen(text,TFT_WHITE,150,295,2);
}

void secondScreen()         //weather data for second screen
{
  citySquare();           // call function to display header
  String desc = jsonDoc["weather"][0]["description"];
  String desc2 = jsonDoc["weather"][1]["description"];
  String id1 = jsonDoc["weather"][1]["id"]; // convert JavaScript objects
  float windspd = jsonDoc["wind"]["speed"]; // to strings or real numbers
  float winddeg = jsonDoc["wind"]["deg"];
  float cloud = jsonDoc["clouds"]["all"];
  unsigned long srise = jsonDoc["sys"]["sunrise"];
  unsigned long sset = jsonDoc["sys"]["sunset"];
  if(id1.length()<1) desc2 = " ";
  screen("Cloud cover",TFT_GREEN,5,55,3);   // cloud cover
  text = String(cloud,1)+"%";
  screen(text,TFT_WHITE,65,85,4);
  screen("wind speed",TFT_BLUE,5,120,2);    // wind speed
  windspd = windspd*3.6;        // convert m/s to km/h
  text = String(windspd,0)+"km/h";
  screen(text,TFT_WHITE,20,145,2);
  screen("direct",TFT_RED,140,120,2);   // wind direction
  text = String(winddeg,0);
  if(winddeg<10) text = " "+text;
  if(winddeg<100) text = " "+text;   
  screen(text,TFT_WHITE,152,145,2);
  screen("o",TFT_WHITE,197,135,2);
  screen("Forecast",TFT_GREEN,50,175,3);    // weather forecast (2)
  addb = blank(desc);
  desc=desc+addb;
  if(desc.length()<13) screen(desc,TFT_WHITE,20,205,3);
  else screen(desc,TFT_WHITE,20,205,2);   // font size depends on text length
  if(desc2 == "null") desc2="";
  addb = blank(desc2);
  desc2=desc2+addb;
  if(desc.length()<13 && desc2.length()<13) 
    screen(desc2,TFT_WHITE,5,235,3);
  else screen(desc2,TFT_WHITE,5,235,2);
  screen("sunrise",TFT_GREEN,20,270,2);   // sunrise time
  text = String(minute(srise));
  if(minute(srise)<10) text = "0"+text;
  text = String(hour(srise)+1)+":"+text;
  screen(text,TFT_WHITE,40,295,2);
  screen("sunset",TFT_GREEN,140,270,2);   // sunset time
  text = String(minute(sset));
  if(minute(sset)<10) text = "0"+text;
  text = String(hour(sset)+1)+":"+text;
  screen(text,TFT_WHITE,150,295,2);
}

void citySquare()         // display header and city abbreviations
{
  tft.fillRect(2,2,235,315,TFT_BLACK);    // clear screen apart from frame
// display city abbreviations
  for (int i=0; i<Ncity; i++) screen(city[i],TFT_YELLOW,210,10+i*25,2);
  for (int i=0; i<Ncity; i++) tft.drawRect(208,8+i*25,29,19,TFT_BLACK);
// draw rectangle for selected city
  tft.drawRect(208,8+cityNow*25,29,19,TFT_WHITE);
  screen("X",TFT_YELLOW,7,31,2);      // draw X with circle
  tft.drawCircle(11,37,11,TFT_WHITE);
  unsigned long stime = jsonDoc["dt"];    // time in secs since 1 Jan 1970
  hr = hour(stime)+1;
  mn = minute(stime);
  dy = day(stime);          // convert time
  wkdy = days[weekday(stime)];
  mth = mths[month(stime)];
  text = wkdy+" "+String(dy)+" "+mth;   // display day, date and time
  screen(text,TFT_YELLOW,10,5,2);
  text = "at "+String(hr)+":"+String(mn);
  if(mn<10) text = "at "+String(hr)+":0"+String(mn);
  screen(text,TFT_YELLOW,60,30,2);
}
              // function to position and display strings
void screen(String text, unsigned int color, int x, int y, int size)
{
  tft.setCursor(x, y);          // position cursor
  tft.setTextColor(color,TFT_BLACK);    // background color: black
  tft.setTextSize(size);
  tft.print(text);
}

String blank(String txt)      // function to add spaces to text
{
  String addb = "";
  int len = 12-txt.length();        // add up to 11 spaces
  for (int i=0;i<len;i++) addb=addb+" ";
  return addb;
}
