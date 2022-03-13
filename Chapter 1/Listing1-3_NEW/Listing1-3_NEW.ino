/*******************************************************************************
 * Sketch name: Internet radio with screen and rotary encoder
 * Description: Internet radio with ESP32 developement board
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    1 - Internet radio
 ******************************************************************************/
#include <ArduinoOTA.h>          // include OTA library
#include <ESPmDNS.h>        // and mDNS libraries
#include <WiFiUdp.h>          // include Wi-FI UDP library 
#include <vs1053_ext.h>          // include ESP32 VS1053_ext,
#include <WiFi.h>           //  WiFi and
#include <Adafruit_ST7735.h>        //  Adafruit_ST7735 libraries
int CS = 0;
int DCS = 2;            // define VS1053 decoder pins
int DREQ = 4;
VS1053 decoder(CS, DCS, DREQ);      // associate decoder with VS1053
//char ssid[] = "xxxx";         // change xxxx to Wi-Fi ssid
//char password[] = "xxxx";       // change xxxx to Wi-Fi password
#include <ssid_password.h>
const int maxStation = 8;        // number of radio stations
String stationName[] = {"Volume",     // first item on menu
"1940 UK", "Berlin", "Bayern3", "Classic", "BBC4",
"Vermont", "Trondeim", "Virgin"};
char * URL[maxStation] = {        // radio station URLs
"1940sradio1.co.uk:8100/stream/1/",
"irmedia.streamabc.net/irm-bbrberlinclub-mp3-128-4574783", // Berlin
"dispatcher.rndfnk.com/br/br3/live/aac/low?aggregator=radio-de", // Bayern3
"media-ice.musicradio.com/ClassicFM",  // media-ice.musicradio.com/ClassicFMMP3 higher quality
"stream.live.vc.bbcmedia.co.uk/bbc_radio_fourfm",
"vpr.streamguys1.com/vpr64.mp3",  // Vermont
"stream.radiometro.no/metro128.mp3", // Trondheim
"radio.virginradio.co.uk/stream"
};
int TFT_CS = 22;
int DCpin = 3;          // define ST7735 TFT screen pins
int RSTpin = 1;         // associate tft with Adafruit ST7735
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, DCpin, RSTpin);
int CLKpin = 25;
int DTpin = 26;
int SWpin = 27;           // define rotary encoder pins
int oldRow = 0;
int newRow = 1;
int menuItem, val, upLimit;
int displayVol[] =          // define volume values
{0,50,60,65,70,75,80,82,84,86,88,90,91,92,93,94,95,96,97,98,99,100};
int volume = 0;
int newVolume = 10;         // volume level at start up
int station = 0;
int newStation = 3;         // station number at start up
int textlen, textrows;
String showstatn, showtitle, showurl, text, text1, text2;
volatile int change = 0;        // rotary encoder variables
volatile int pressed = 0;
volatile int vals[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,0,-1,0};
volatile int score = 0;
volatile int oldState = 0;
volatile int turn;

void setup()
{
  tft.initR(INITR_BLACKTAB);        // initialise screen
  tft.fillScreen(ST7735_BLACK);     // clear screen
  tft.setRotation(1);         // orientate ST7735 screen
  tft.setTextSize(2);         // set screen text size
  tft.drawRect(0,0,158,126,ST7735_WHITE);   // draw white frame line
  tft.drawRect(2,2,154,122,ST7735_RED);   //  and second frame line
  tft.setCursor(0, 50);
  SPI.begin();            // initialise SPI bus
  WiFi.begin(ssid, password);       // initialise Wi-Fi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    tft.print(".");
  }
  decoder.begin();          // initialise VS0153 decoder
  decoder.setVolume(volume);        // set decoder volume level
  pinMode(CLKpin, INPUT_PULLUP);
  pinMode(DTpin, INPUT_PULLUP);     // rotary encoder uses
  pinMode(SWpin, INPUT_PULLUP);     //  internal pull-up resistors
  attachInterrupt(CLKpin, encoder, CHANGE);
  attachInterrupt(DTpin, encoder, CHANGE);  // attach rotary encoder interrupts
  attachInterrupt(SWpin, swPress, CHANGE);
  ArduinoOTA.setHostname("ESP32Radio");     // name network port
  ArduinoOTA.setPassword("admin1");     // set password
  ArduinoOTA.begin();         // initialise ArduinoOTA
}

void loop()
{
  ArduinoOTA.handle();          // check for OTA updates
  if(pressed == 1)          // switch pin pressed first time
  {               //     to change station or volume
    clearScreen();          // call clearScreen function
    screen();           // call screen function
    menuItem = readMenu(maxStation);    // selected row in menu
  }
  else if (pressed == 2)        // switch pin pressed second time
  {               //  to select station
    if(menuItem > 0)          // station selected
    {
     newStation = menuItem-1;       // selected station in menu
     clearScreen();         // call clearScreen function
     showStation(volume, showstatn, showtitle); // call showStation function
     if(newStation == station) showStation(volume, showstatn, showtitle);
    }               // volume change selected
    else if(menuItem == 0) newVolume = readValue("volume: ", volume, 21, 1);
    pressed = 0;            // reset variable
  }
  else if(pressed > 2) pressed = 0;     // volume changed
  radio();              // call radio function
}

void radio()            // function to connect to
{               //    selected radio station server
  if(station != newStation)       // new station selected
  {
    clearScreen();          // call clearScreen function
    station = newStation;
    showurl = "";
    decoder.connecttohost(URL[station]);    // connect to radio station server
  }
  if(volume != newVolume)       // new volume level selected
  {
    volume = newVolume;
    newRow = station+1;         // retain station number on menu
    decoder.setVolume(volume);      // update VS1053 volume
    clearScreen();          // call clearScreen function
    showStation(volume, showstatn, showtitle);  // call showStation function
  }
  decoder.loop();
}

int readMenu (int rows)     // function to obtain station number on menu
{
  while(pressed < 2)          // while station not selected
  {
    if(change != 0)         // rotary encoder turned
    {
      newRow = oldRow + change;     // retain row number on menu
      newRow = constrain(newRow, 0, rows);
      clearScreen();          // call clearScreen function
      screen();           // call screen function
      oldRow = newRow;
      change = 0;
    }
    delay(10);
  }
  return newRow;            // return row number on menu
}
                // function to obtain volume level
int readValue(String text, int current, int upLimit, int gain)
{
  val = current;            // current volume level
  clearScreen();            // call clearScreen function
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 50);
  tft.print(text);          // display text and current
  tft.print(displayVol[val]);       //  volume value, while
  while(pressed < 3)          //  switch pin is not pressed
  {
    if(change != 0)         // rotary encoded turned
    {
      val = val + change * gain;      // increment volume level
      val = constrain(val, 0, upLimit);   // constrain volume level
      clearScreen();          // call clearScreen function
      tft.setCursor(10, 50);
      tft.print(text);          // display text and
      tft.print(displayVol[val]);     //  new volume value
      change = 0;
    }
    delay(10);
  }
  return val;           // return new volume level
}

void vs1053_showstation(const char * info)
{               // function to obtain station name
  showstatn = String(info);       // station name
  showtitle = "";
  if(showstatn == "No Name") showstatn = stationName[station+1];
  clearScreen();
  showStation(volume, showstatn, showtitle);  // call showStation function
}

void vs1053_showstreamtitle(const char * info)
{               // function to obtain streamed title
  showtitle = String(info);
  clearScreen();
  showStation(volume, showstatn, showtitle);
}

void vs1053_icyurl(const char * info)
{               // function to obtain station URL
  showurl = String(info);
  int i = showurl.indexOf("/");     // position of first / in string
  showurl = showurl.substring(i+2);     // station URL as substring
  clearScreen();
  showStation(volume, showstatn, showtitle);
}

void showStation(int volume, String showstatn, String showtitle)
{   // function to display station name, streamed title and station URL on screen
    tft.setTextColor(ST7735_GREEN);
    tft.setTextSize(1);
    lines(showstatn, 10);       // lines function to display station
    tft.setTextColor(ST7735_YELLOW);
    lines(showtitle, 40);         // lines function to display title
    tft.setTextColor(ST7735_GREEN);
    tft.setCursor(80, 100);       // display volume value
    tft.print("volume: ");tft.print(displayVol[volume]);
    tft.setCursor(5, 110);
    tft.print(showurl);         // display URL
}

void lines(String text, int line)
{         // function to split string into screen sized substrings
  textlen = text.length();        // get string length
  textrows = 1+textlen/23;        // required number of screen rows
  for(int i=0; i<textrows; i++)
  {
    tft.setCursor(10, line + i*10);     // move cursor to next row
    tft.println(text.substring(i*23, (i+1)*23));  // display substring
  }
}

void screen()         // function to display station menu
{
  tft.setTextSize(2);         // increase text size
  tft.setTextColor(ST7735_RED);     // selected station in RED
  tft.setCursor(20, 55); 
  tft.print(stationName[newRow]);     // display station name
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE);     // all other stations in WHITE
  for (int i=1; i<4; i++)       // display other station names
  {
    tft.setCursor(30, 50 - i*12);     // above selected station
    if(newRow-i >=0) tft.print(stationName[newRow-i]);
    tft.setCursor(30, 65 + i*12);       // below selected station
    if(newRow+i < maxStation+1) tft.print(stationName[newRow+i]);
  }
}

void clearScreen()        // function to clear screen
{             // by displaying a BLACK rectangle
  tft.fillRect(3,3,152,120,ST7735_BLACK);
}

IRAM_ATTR void encoder()      // function to count rotary encoder turns
{
  int newState = (oldState<<2)+(digitalRead(CLKpin)<<1)+digitalRead(DTpin);
  score = score + vals[newState];   // allocate score from array
  oldState = newState % 4;      // remainder to leave new CLK and DT
  if(score == 2 || score == -2)   // 2 steps for complete rotation
  {
    change = score/2;       // unit change per two steps
    score = 0;          // reset score
  }
}

IRAM_ATTR void swPress()      // function to count switch presses
{     // pressed = 1, 2, 3 to change station, station selected, volume changed
  if(digitalRead(SWpin) == HIGH) pressed = pressed + 1;
}
