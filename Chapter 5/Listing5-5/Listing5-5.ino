/*******************************************************************************
 * Sketch name: MP3 player alarm
 * Description: Sound recoding on MP3 player triggered by ultrasonic distance
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    5 - MP3 player
 ******************************************************************************/

#include<SoftwareSerial.h>        // include SoftwareSerial library
SoftwareSerial SoftSer(D6, D7);     // define SoftSer TX pin
#include <NewPingESP8266.h>       // include NewPingESP8266 lib
int trigPin = D1;           // HC-SR04 trigger pin
int echoPin = D2;           // HC-SR04 echo pin
int maxdist = 200;          // set max scan distance (cm)
int echoTime;
float distance;           // scanned distance (cm)
// associate sonar with NewPing
NewPingESP8266 sonar (trigPin, echoPin, maxdist);
int LEDpin = D3;            // define LED pin
int alarmPin = D4;          // define alarm switch pin
unsigned int checksum;
byte highChk, lowChk;         // control command template
byte serialCom[10] = {0x7E,0xFF,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0xEF};
byte buffer[10];
volatile int alarmSet = 0;          // set alarm state
String fileName[] = {         // file names in numerical order
"0001 alarm off",
"0002 alarm on",
"0003 someone entered the room",
"0004 close the door please",
"0005 press switch to reset the alarm"
};

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  SoftSer.begin(9600);          // software Serial baud rate
  pinMode(trigPin, OUTPUT);       // define trigger pin as output
  pinMode(LEDpin, OUTPUT);        // define LEDpin as output
  pinMode(alarmPin, INPUT_PULLUP);      // alarm pin uses pull-up resistor
  attachInterrupt(digitalPinToInterrupt(alarmPin), alarmISR, FALLING);
}

void loop()
{
  echoTime = sonar.ping();        // echo time (s)
  distance = (echoTime/2.0)*0.0343;     // distance to target
  Serial.println(distance);       // play audio files if
  if(distance < 100 && alarmSet == 1) play(); // distance < 100 and alarm set
  delay(2000);            // delay between readings
}

void play()
{
  cmd(0x06, 10);            // volume to 10 (range 0 to 30)
  cmd(0x12, 3);           // play audio file named 0003
  delay(2000);            // interval between audio files
  cmd(0x12, 4);           // play audio file named 0004
  delay(2000); 
  cmd(0x12, 5);           // play audio file named 0005
  delay(2500);
  alarmISR();           // turn alarm off
}

void cmd(byte CMD, byte param2)
{
  checksum = -(0xFF + 0x06 + CMD + 0x00 + 0x00 + param2); // build checksum
  highChk = highByte(checksum);     // split checksum into
  lowChk = lowByte(checksum);       //  high byte and low bytes
  serialCom[3] = CMD;
  serialCom[6] = param2;        // components of command
  serialCom[7] = highChk;
  serialCom[8] = lowChk;        // transmit command to MP3
  for (int i=0; i<10; i++) SoftSer.write(serialCom[i]);
}

IRAM_ATTR void alarmISR()
{
  alarmSet = 1 - alarmSet;        // turn off (0) or on (1) the alarm
  digitalWrite(LEDpin, alarmSet);     // turn off or on the LED
  cmd(0x12, alarmSet+1);        // play audio file 0001 or 0002
}
