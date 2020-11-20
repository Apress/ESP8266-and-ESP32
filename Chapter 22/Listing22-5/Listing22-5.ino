/*******************************************************************************
 * Sketch name: Transmit and receive with BLE communication
 * Description: Connecto to BLE device, transmit data and receive message from device
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    22 - SP32 microcontroller features
 ******************************************************************************/

#include <BLEDevice.h>            // include BLE libraries
#include <BLE2902.h>
BLEServer * pServer;            // define BLE server,
BLEService * pService;            //   BLE service,
BLECharacteristic * pTXChar;          //   BLE transmit and
BLECharacteristic * pRXChar;          //   receive characteristics,
BLEDescriptor * pTXDesc;          //   transmit descriptor
int devConnect = 0;
int devNotify = 0;
int LEDpin = 25;
int LEDstate = 0;
float value;
char valueStr[8];
char SERVICE_UUID[] = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"; // UUIDs
char RXChar_UUID[] =  "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
char TXChar_UUID[] =  "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";

void changeLED()            // function to change LED state
{
  LEDstate = 1 - LEDstate;          // turn on or off LED
  digitalWrite(LEDpin, LEDstate);
}

class ServerConnect: public BLEServerCallbacks    // to check if connected
{
  void onConnect(BLEServer * pServer) {devConnect = 1;}
  void onDisconnect(BLEServer * pServer) {devConnect = 0;}
};

class RXCallback: public BLECharacteristicCallbacks // receive client data
{
  void onWrite(BLECharacteristic * pCharacteristic)
  {
    std::string RXstr = pCharacteristic->getValue();
    if (RXstr.length() > 0)         // client data available
    {
      Serial.print("Received: ");       // read client data
      for (int i=0; i<RXstr.length(); i++) Serial.print(RXstr[i]);
      Serial.println();
      if(RXstr == "LED") changeLED();   // call changeLED function
    }
  }
};

class NotifyCallback: public BLEDescriptorCallbacks // client data notification
{
  void onWrite(BLEDescriptor * pDescriptor)
  {
    uint8_t * TXvalue = pDescriptor->getValue();  // obtain TX descriptor
    devNotify = 0;
    if (pDescriptor->getLength() > 0)     // client data available
    {
      if(TXvalue[0] == 1) devNotify = 1;      // update data notification
      Serial.print("Notify: ");Serial.println(devNotify);
    }
  }
};

void setup()
{
  Serial.begin(115200);           // Serial monitor baud rate
  pinMode(LEDpin, OUTPUT);
  BLEDevice::init("ESP32");         // define BLE device
  pServer = BLEDevice::createServer();      // define BLE server
  pServer->setCallbacks(new ServerConnect());   // check if connected
  pService = pServer->createService(SERVICE_UUID);  // define BLE service
  pTXChar = pService->createCharacteristic(   // define TX characteristic
TXChar_UUID, BLECharacteristic::PROPERTY_NOTIFY);
  pTXDesc = new BLE2902();          // define TX descriptor
  pTXDesc->setCallbacks(new NotifyCallback());    // attach notify callback
  pTXChar->addDescriptor(pTXDesc);
  pRXChar = pService->createCharacteristic(   // define RX characteristic
RXChar_UUID, BLECharacteristic::PROPERTY_WRITE);
  pRXChar->setCallbacks(new RXCallback());    // attach RX callback
  pService->start();            // start service
  pServer->getAdvertising()->start();     // advertise service
  Serial.println("Waiting for client to connect");
}

void loop()
{               // if the client is connected and
  if(devConnect == 1 && devNotify == 1)   //  requests data notification
  {
    value = random(10, 200)*1.5;      // generate real number
    dtostrf(value, 8, 1, valueStr);     // convert 8-char string with 1DP
    pTXChar->setValue(valueStr);      // update characteristic
    pTXChar->notify();          // notify client
    Serial.print(value);Serial.print("\t");Serial.println(valueStr);
    delay(3000);            // delay between updates
  }
}
