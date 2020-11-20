/*******************************************************************************
 * Sketch name: Speaking clock
 * Description: Sound recoding of current time triggered by switch
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    5 - MP3 player
 ******************************************************************************/

#include<SoftwareSerial.h>        // include SoftwareSerial library
SoftwareSerial SoftSer(D6, D7);     // define SoftSer TX pin
#include <MD_DS3231.h>          // include MD_DS3231 library
unsigned int checksum;
byte highChk, lowChk;
byte serialCom[10] ={0x7E,0xFF,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0xEF};
byte buffer[10];
int switchPin = D4;         // define switch pin
int val, deg;

void setup()
{
  SoftSer.begin(9600);          // software Serial baud rate
  pinMode(switchPin, INPUT_PULLUP);     // switch pin uses pull-up resistor
  cmd(0x06, 10);            // set volume to 10 (range 0 - 30)
  RTC.control(DS3231_12H, DS3231_OFF);    // 24 hour clock
// define RTC date of Wednesday 7 September 2020 at 20:37:50 (hh:mm:ss)
/*    // instructions between /* and */ to set the time are commented out
  RTC.yyyy = 2020;          // year
  RTC.mm = 9;           // month
  RTC.dd = 7;           // day
  RTC.h = 20;           // hour in 24 hour format
  RTC.m = 37;           // minutes
  RTC.s = 50;           // seconds, allow 30s to compile
  RTC.dow = 4;            // day of week, Sunday = 1
  RTC.writeTime();
*/
}

void loop()
{
  if(digitalRead(switchPin) == LOW)     // switch is pressed
  {
    speak(25);            // MP3 play "the time is"
    RTC.readTime();         // components of date and time
    speak20(RTC.h);         // MP3 play the hour
    if(RTC.m == 0) speak(28);       // MP3 play "o'clock"
    else if(RTC.m <10)
    {
      speak(24);            // MP3 play "zero"
      speak(RTC.m);         // MP3 play minute < 10
    }
    else if(RTC.m <21) speak(RTC.m);    // MP3 play minute <21
    else
    {
      speak(RTC.m/10 + 18);       // MP3 play "30 40 or 50 mins"
      speak(RTC.m % 10);        // MP3 play minute < 10
    }
    deg = round(RTC.readTempRegister());    // temperature measurement
    speak(27);            // MP3 play "and"
    speak20(deg);           // MP3 play the temperature <29°
    speak(26);            // MP3 play "degrees Celsius"
  }
}

void speak(int file)          // function to play MP3 file
{
  if(file == 27) delay(200);        // delay before playing "and"
  cmd(0x03, file);
  delay(300);           // time for short track to play
  if(file == 25 || file == 27) delay(300);  // delay for "the time is" or "and"
}

void speak20(int val)         // function to play combination
{               //  of "20" and units
  if(val < 21) speak(val);        // MP3 play number < 21
  else
  {
    speak(20);            // MP3 play "20"
    speak(val % 20);          // MP3 play track numbered
  }               // remainder after dividing by 20
}

void cmd(byte CMD, byte param2)
{
  delay(500);           // stop repeated button push
  checksum = -(0xFF + 0x06 + CMD + 0x00 + 0x00 + param2); // build checksum
  highChk = highByte(checksum);     // split checksum into
  lowChk = lowByte(checksum);       //  high byte and low bytes
  serialCom[3] = CMD;
  serialCom[6] = param2;        // components of command
  serialCom[7] = highChk;
  serialCom[8] = lowChk;        // transmit command to MP3
  for (int i = 0; i<10; i++) SoftSer.write(serialCom[i]);
  delay(20);            // time to load command
}
