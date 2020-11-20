/*******************************************************************************
 * Sketch name: MP3 player
 * Description: Button control of MP3 player
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    5 - MP3 player
 ******************************************************************************/

#include<SoftwareSerial.h>        // include SoftwareSerial library
SoftwareSerial SoftSer(D6, D7);     // define SoftSer RX, TX pins
int nextPin = D4;
int volUp = D3;
int volDown = D2;           // define control pins
int EQpin = D8;
int busyPin = D5;
int EQstate = 0;            // equaliser settings
String EQ[] = {"Normal","Pop","Rock","Jazz","Classic","Bass"};
unsigned long timed = 0;
unsigned int checksum;
byte highChk, lowChk;         // control command template
byte serialCom[10] = {0x7E,0xFF,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0xEF};
// start version length CMD feedback para[1, 2] checksum[high, low] end
byte buffer[10];
int fileMin = 9;            // lowest and highest file number
int fileMax = 13;           //  on micro SD card
int file = fileMin;
String fileName[] = {       // file names in order loaded on SD card
"0012 Nina Simone - My baby just cares for me",
"0011 Reamonn - Supergirl",
"0013 Spider Murphy Gang - Ich Grüsse Alle Und Den Rest Der Welt",
"0009 Proclaimers - I'm Gonna Be (500 Miles)",
"0010 Railroad Earth - The Good Life"
};
volatile int finish;        // variable in loop and ISR functions
int track;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  SoftSer.begin(9600);          // and software Serial baud rate
  pinMode(nextPin, INPUT_PULLUP);     // control pins use internal
  pinMode(volUp, INPUT_PULLUP);     //  pull-up resistors
  pinMode(volDown, INPUT_PULLUP);
// interrupt finished, BUSY pin HIGH
  attachInterrupt(digitalPinToInterrupt(busyPin), finished, RISING);
  cmd(0x48, 0);           // get number of files on SD card
  cmd(0x06, 10);            // set volume to 10 (range 0 - 30)
  cmd(0x43, 0);           // get volume value
  cmd(0x12, file);          // play first audio file
  finish = 0;           // set finish variable
  cmd(0x4C, 0);           // get track number
  timed = millis();         // start timer
}

void loop()
{             // next file selected or current file ended
  if(digitalRead(nextPin) == LOW || finish == 1)
  {
    file = file+1;          // increment file name
    if(file > fileMax) file = fileMin;    // constrain file name <= fileMax
    cmd(0x12, file);          // play next audio file
    finish = 0;           // set finish variable
    cmd(0x4C, 0);           // get track number
  }
  else if(digitalRead(volUp) == LOW)    // increase volume is selected
  {
    cmd(0x04, 0);           // increase volume
    cmd(0x43, 0);           // get volume value
  }
  else if(digitalRead(volDown) == LOW)    // decrease volume is selected
  {
    cmd(0x05, 0);           // decrease volume
    cmd(0x43, 0);
  }
  else if(digitalRead(EQpin) == HIGH)   // change equaliser is selected
  {               // when pin state is HIGH
    EQstate = EQstate+1;        // increment equaliser
    if(EQstate > 5) EQstate = 0;      // constrain equaliser value
    Serial.println(EQ[EQstate]);
    cmd(0x07, EQstate);         // change equaliser setting
  }
}

void cmd(byte CMD, byte param2)     // command function
{
  delay(100);           // delay to debounce button
  checksum = -(0xFF + 0x06 + CMD + 0x00 + 0x00 + param2); // build checksum
  highChk = highByte(checksum);     // split checksum into
  lowChk = lowByte(checksum);       //  high byte and low bytes
  serialCom[3] = CMD;
  serialCom[6] = param2;        // command components
  serialCom[7] = highChk;
  serialCom[8] = lowChk;        // transmit command to MP3
  for (int i=0; i<10; i++) SoftSer.write(serialCom[i]);
  delay(100);           // receive command from MP3
  for (int i=0; i<10; i++) buffer[i] = SoftSer.read();
  delay(100);
  if(CMD == 0x12)           // play next audio file
  {
    Serial.print("finished track ");Serial.print(track);
    Serial.print("\ttime ");Serial.print((millis() - timed)/1000);
    Serial.println("s");        // display audio play time
    timed= millis();          // reset e
  }
  else if(CMD == 0x43)          // get volume
  {
    Serial.print("volume ");        // display volume value
    Serial.println(buffer[6]);
  }
   else if(CMD == 0x48)         // get number of files on SD card
  {
    if(buffer[6]<2)         // no audio files present
    {
      Serial.println("SD card not inserted ");
      Serial.println("insert SD card and reset microcontroller");
      for(;;) delay(1000);        // do nothing
    }
  }
  else if(CMD == 0x4C)          // get track number
  {
    track = buffer[6];          // display track number of file
    Serial.print("playing track ");Serial.print(track);
    Serial.print("\t\t");Serial.println(fileName[track-1]);
  }             // array starts at [0], but track starts at [1]
}

IRAM_ATTR void finished()         // ISR
{
  finish = 1;             // set finish variable
}
