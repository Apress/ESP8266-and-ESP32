/*******************************************************************************
 * Sketch name: BLE communication as a server
 * Description: Transmit generated "environmental" data with BLE
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    22 - ESP32 microcontroller features
 ******************************************************************************/

#include <BLEDevice.h>          // include BLE libraries
#include <BLE2902.h>
BLEServer * pServer;          // define BLE server,
BLEService * pService;          //  BLE service and
BLECharacteristic * pChar;        //  BLE Characteristic
int devConnect = 0;
#define SERVICE_UUID BLEUUID((uint16_t)0x181A)    // environmental service
BLECharacteristic tempChar(BLEUUID((uint16_t)0x2A6E),
   BLECharacteristic::PROPERTY_NOTIFY);
BLECharacteristic UVChar  (BLEUUID((uint16_t)0x2A76),
   BLECharacteristic::PROPERTY_NOTIFY);
BLECharacteristic heatChar(BLEUUID((uint16_t)0x2A7A),
   BLECharacteristic::PROPERTY_NOTIFY);
class ServerConnect: public BLEServerCallbacks    // to check if connected
{
  void onConnect(BLEServer * pServer) {devConnect = 1;}
  void onDisconnect(BLEServer * pServer) {devConnect = 0;}
};

float temp = 0;
uint8_t tempData[2];
uint16_t tempValue;
int UV = 0, heat;

void setup()
{
  Serial.begin(115200);           // Serial monitor baud rate
  BLEDevice::init("ESP32");         // define BLE device
  pServer = BLEDevice::createServer();      // define BLE server
  pServer->setCallbacks(new ServerConnect());   // check if connected
  pService = pServer->createService(SERVICE_UUID);  //define BLE service
  pService->addCharacteristic(&tempChar);     // define temperature
  tempChar.addDescriptor(new BLE2902());      //  characteristic
  pService->addCharacteristic(&UVChar);     // define UV index
  UVChar.addDescriptor(new BLE2902());      //  characteristic
  pService->addCharacteristic(&heatChar);     // defile heat index
  heatChar.addDescriptor(new BLE2902());      //  characteristic
  pServer->getAdvertising()->addServiceUUID(SERVICE_UUID);
  pService->start();            // start service
  pServer->getAdvertising()->start();     // advertise service
  Serial.println("Waiting for client to connect");
}

void loop()
{
  if(devConnect == 1)         // if the client is connected
  {
    temp = temp + 1.11;
    tempValue = (uint16_t)(temp*100);   // convert to uint16_t with 2DP
    tempData[0]= tempValue;       // LSB (least significant byte)
    tempData[1]= tempValue >> 8;      // MSB (most significant byte)
    tempChar.setValue(tempData, 2);     // update characteristic
    tempChar.notify();          // notify client
    heat = (uint8_t)temp;       // convert to uint8_t with 0DP
    heatChar.setValue(heat);
    heatChar.notify();
    UV = UV + 1;
    UVChar.setValue((uint8_t*)&UV, 4);    // point to address of UV
    UVChar.notify();
    Serial.print(temp);Serial.print("\t");  // display values on Monitor
    Serial.print(heat);Serial.print("\t");
    Serial.println(UV);
    delay(1000);            // delay between sensor readings
  }
}
