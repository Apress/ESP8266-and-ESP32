/*******************************************************************************
 * Sketch name: BLE watch control
 * Description: Scan for specific BLE device
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    22 - ESP32 microcontroller features
 ******************************************************************************/

#include <BLEDevice.h>          // include BLE library
BLEScan * pBLEScan;         // pointer to BLE scanner
BLEAddress * pAddress;          //  and to BLE address
BLEScanResults devices;
String watch = "d5:db:a5:45:99:2f";     // change upper to lower case
String scan;
int scanTime = 3;           // scan devices for 3s
int paired = 0, lastPair = 0;
int LEDpin = 25;            // define LED pin

class watchCallback: public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertised) // BLE advertising devices
  {               // option to display device data
//  Serial.printf("found device: %s \n", advertised.toString().c_str());
    pAddress = new BLEAddress(advertised.getAddress()); // BLE address
    scan = pAddress->toString().c_str();        // convert to string
    if(scan == watch) paired = 1;     // device matches watch address
  }
};

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  pinMode(LEDpin, OUTPUT);        // LED pin as OUTPUT
  BLEDevice::init("");          // initialise BLE client
  pBLEScan = BLEDevice::getScan();      //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new watchCallback());
  pBLEScan->setActiveScan(true);
}

void loop()
{
  devices = pBLEScan->start(scanTime, false); // start scanning
//  Serial.print("scanned devices ");Serial.print(devices.getCount());
  digitalWrite(LEDpin, paired);     // turn on or off LED
  if(paired == 0 && lastPair == 1) digitalWrite(LEDpin, 1);
  lastPair = paired;        // 2 consecutive non-pairings to turn off
  paired = 0;           // reset paired variable
  pBLEScan->clearResults();       // delete scan results
}
