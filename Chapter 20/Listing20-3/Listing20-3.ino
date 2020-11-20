/*******************************************************************************
 * Sketch name: Write, read and append file with SPIFFS for ESP8266 microcontroller
 * Description: Save data on SPIFFS
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    20 - OTA and saving data to EEPROM, SPIFFS and Excel
 ******************************************************************************/

#include <LittleFS.h>          // include LittleFS library
String filename = "/temp/testfile.txt";   // structure /dir/file
String newname = "/temp/newfile.txt";

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  if(LittleFS.begin()) Serial.println("initialised OK");
  dirContent("/");          // contents of main directory
  dirContent("/temp");          // contents of sub directory
  File file = LittleFS.open(filename, "w"); // open file to write
  file.println("ABC");
  file.println("123");          // instead of print("xxx\n")
  file.close();
  fileContent(filename);        // function display file content
  dirContent("/temp");
  file = LittleFS.open(filename, "a");    // append to file
  file.println("XYZ");
  file.close();
  LittleFS.rename(filename, newname);   // change filename not directory
  fileContent(newname);
  dirContent("/temp");
  if(LittleFS.exists(filename)) LittleFS.remove(filename);  // delete file
}

void dirContent(String dname)     // function to display directory content
{
  Serial.print(dname);Serial.println(" content");
  Dir dir = LittleFS.openDir(dname);
  while(dir.next())
  {
    File file = dir.openFile("r");          // read file
    Serial.print("file ");Serial.print(file.name());Serial.print("\t");
    Serial.print("size ");Serial.println(file.size());  // filesize
  }
}

void fileContent(String fname)    // function to display file content
{
  File file = LittleFS.open(fname, "r");
  while(file.available()) Serial.write(file.read());
  file.close();
}

void loop()             // nothing in loop function
{}
