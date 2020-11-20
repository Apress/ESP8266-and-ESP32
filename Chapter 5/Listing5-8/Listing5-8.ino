/*******************************************************************************
 * Sketch name: ISD1820 record and playback module
 * Description: Recording and playback of sounds
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    5 - MP3 player
 ******************************************************************************/

int playPin = D3;            // define playback pin
int recPin = D4;            // define record pin
char data;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  Serial.println("Enter r to record (10 seconds) or p to playback");
  digitalWrite(playPin, LOW);       // avoid playPin going HIGH
  pinMode(playPin, OUTPUT);       // define playPin and recPin
  pinMode(recPin, OUTPUT);        //  as OUTPUT
}

void loop()
{
  while(Serial.available() > 0)     // if data available in Serial buffer
  {
    data = Serial.read();       // read Serial buffer
    if(data == 'r')
    {
      Serial.println("recording while light is on");
      digitalWrite(recPin, HIGH);     // HIGH to activate recording
      delay(10000);         // recording time of 10s
      digitalWrite(recPin, LOW);      // reset to LOW signal
    }
    else if(data == 'p')
    {
      Serial.println("playback");
      digitalWrite(playPin, HIGH);      // HIGH to activate playback
      delay(10);            // short delay of 10ms
      digitalWrite(playPin, LOW);     // reset to LOW signal
    }
  }
}
