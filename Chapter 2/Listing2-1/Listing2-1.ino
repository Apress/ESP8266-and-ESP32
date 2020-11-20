/*******************************************************************************
 * Sketch name: Taking a photo and saving to micro-SD card
 * Description: N photos taken at set interval and saved to micro-SD card
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    2 - Intranet camera
 ******************************************************************************/

#include <esp_camera.h>          // include esp_camera library
#include <SD_MMC.h>         // include SD_MMC library
#include <EEPROM.h>         // include EEPROM
#include "config_pins.h"        // configure instructions tab
uint8_t SDtype;
int SDpics;             // number of pictures on SD card
int maxPhoto = 0;           // maximum number of photos
int Nphoto = 0;           // number of photos taken
int photoTime = 2000;         // delay (ms) between photos
String filename;
unsigned long nowTime, lastTime = 0;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  Serial.println("\n\nenter number of required photos");
  Serial.println("\n\nsettling down for 2s"); // time to settle vibration
  delay(2000);
  Serial.println("initialising camera, then take photos");
  configCamera();           // functions to configure camera
  initSDcard();           // and to initialise micro-SD card
  EEPROM.begin(1);          // EEPROM with one record
  SDpics = EEPROM.read(0);        // number of saved pictures
}

void loop()
{
  while (Serial.available()>0)      // maximum photo number
  {
    maxPhoto = Serial.parseInt();     // parsed from Serial buffer
    Nphoto = 0;
  }           // if photo number < maximum photo number
  nowTime = millis();         // take photo after photoTime
  if((nowTime - lastTime > photoTime) && (Nphoto < maxPhoto))
  {
    Nphoto++;           // increment photo number
    takePhoto();            // call function to take photo
    lastTime = millis();        // update time of photo
  }
}

void initSDcard()           // function to initialise SD card
{
  if(!SD_MMC.begin())         // check SD card in position
  {
    Serial.println("error loading SD card");
    return;
  }
  SDtype = SD_MMC.cardType();       // obtain SD card type
  if(SDtype == CARD_NONE)
  {
    Serial.println("insert SD Card");
    return;
  }
  Serial.print("SD card type: ");
  if(SDtype == CARD_MMC) Serial.println("MMC");
  else if(SDtype == CARD_SD) Serial.println("SDSC");
  else if(SDtype == CARD_SDHC) Serial.println("SDHC");
  else Serial.println("UNKNOWN");
}

void takePhoto()            // function to take and save photo
{
  camera_fb_t * frame = NULL;       // associate fb with esp_camera
  frame = esp_camera_fb_get();        // take photo with camera
  if(!frame)
  {
    Serial.println("photo capture error");
    return;
  }
  SDpics ++;              // increase picture number
  filename = "/picture" + String(SDpics) +".jpg"; // generate JPEG filename
  fs::FS & fs = SD_MMC;
  File file = fs.open(filename.c_str(), FILE_WRITE);  // access SD card
  if(!file) Serial.println("file save error");
  else
  {
    file.write(frame->buf, frame->len);     // save file to SD card
    Serial.print("Picture filename: ");Serial.println(filename);
    EEPROM.write(0, SDpics);          // update EEPROM with
    EEPROM.commit();            //  picture number
  }
  file.close();             // close file on SD card
  esp_camera_fb_return(frame);    //return frame buffer to driver for reuse
}
