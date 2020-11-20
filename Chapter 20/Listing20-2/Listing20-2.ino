/*******************************************************************************
 * Sketch name: Saving data in EEPROM
 * Description: Save data on microcontroller for storage when switched off
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    20 - OTA and saving data to EEPROM, SPIFFS and Excel
 ******************************************************************************/

#include <EEPROM.h>          // include EEPROM library
int EEaddress, records;
unsigned long seconds, nowTime, lastTime = 0;
char cmd = ' ';           // command character

typedef struct            // structure to hold data record
{
  float UV;             // a real number (4 bytes) and an
  int minuteTime;           // integer (4 bytes in EEPROM)
  int secondTime;
} dataStruct;
dataStruct data;            // number of bytes to store data
int Nbytes = sizeof(float) + sizeof(data.minuteTime) + sizeof(int);

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  EEPROM.begin(1000);         // set EEPROM capacity
  EEPROM.put(0, 0);         // set record number to zero
  EEPROM.commit();          // write to EEPROM
  Serial.println("Enter R: record, D: display or Z: zero UV records");
}

void loop()
{               // command from Serial buffer
  if(Serial.available() > 0) cmd = Serial.read();
  nowTime = millis();         // start of time interval
  if((nowTime - lastTime > 5000) && (cmd == 'R')) // collect data every 5s
  {
    data.UV = analogRead(A0)*3200.0/1024;   // convert reading to mV
    seconds = (nowTime/1000);
    data.minuteTime = seconds / 60;     // calculate elapsed minutes
    data.secondTime = seconds % 60;     //  and seconds
    EEPROM.get(0, records);       // number of EEPROM records
    EEaddress = records * Nbytes + 4;   // address of new record
    if((EEaddress + Nbytes) > EEPROM.length())  // check if exceeding the
    {               //  set EEPROM capacity
      records = 0;          // reset EEPROM record number
      EEaddress = 4;          // avoid over-flowing EEPROM
    }
    records++;            // increment number of records
    EEPROM.put(0, records);       // update number of records
    EEPROM.put(EEaddress, data);      // write data to EEPROM
    EEPROM.commit();
    Serial.print("UV index ");Serial.println(data.UV);  // display data
    lastTime = nowTime;         // update time interval
  }
  if(cmd == 'D')            // display data held on EEPROM
  {
    records = EEPROM.get(0, records);   // number of EEPROM records
    for (int i=0; i<records; i++)
    {
      EEaddress = i * Nbytes + 4;     // EEPROM address of ith record
      EEPROM.get(EEaddress, data);      // read and display EEPROM
      Serial.print(data.minuteTime);Serial.print(":");
      Serial.print(data.secondTime);Serial.print("\t");
      Serial.println(data.UV);
    }
    cmd = ' ';            // reset command
    Serial.println("Enter R: record, D: display or Z: zero UV record");
  }
  if(cmd == 'Z')            // command to reset records
  {
    EEPROM.put(0, 0);         // set record number to zero
    EEPROM.commit();
    cmd = ' ';
    Serial.println("Enter R: record, D: display or Z: zero UV record");
  }
}
