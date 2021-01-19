/*******************************************************************************
 * Sketch name: ESP-MESH and Bluetooth communication (node 3)
 * Description: Reception of LED state over ESP-MESH with callback
 *              Bluetooth communication between ESP32 and app
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    ESP-MESH
 ******************************************************************************/
 #include <painlessMesh.h>        // differences from Listing 2
String ssid = "meshSSID";
String password = "meshPass";
int port = 5555;
int LEDpin = 27;            // define ESP32 LED pin
int LEDstate, oldLEDstate;
DynamicJsonDocument jsonDoc(256);

Scheduler scheduler;
painlessMesh mesh;
void sendMessage();
Task taskSend(TASK_SECOND * 1 , TASK_FOREVER, &sendMessage);

#include <BluetoothSerial.h>        // include Bluetooth library
BluetoothSerial SerialBT;       // associate SerialBT with library
String sendBT, oldBT = "";        // Bluetooth message

void setup()
{
  Serial.begin(115200);
  pinMode(LEDpin, OUTPUT);        // LED pin as OUTPUT
  mesh.init(ssid, password, &scheduler, port);
  mesh.onReceive(&recvMessage);
  scheduler.addTask(taskSend);
  taskSend.enable();
  SerialBT.begin("ESP32 right");      // identify Bluetooth device
}

void sendMessage()
{
  float value = random(0, 100);
  jsonDoc["value"] = value;       // update name and value pairs
  jsonDoc["msg"] = sendBT;        // for value and BT message
  String sendMsg;
  serializeJson(jsonDoc, sendMsg);
  mesh.sendBroadcast(sendMsg);
  sendBT = "";            // reset message to Android tablet
  taskSend.setInterval((TASK_SECOND * 3));
}

void recvMessage(uint32_t sender, String &recvMsg)
{
  deserializeJson(jsonDoc, recvMsg);
  String recvBT = jsonDoc["msg"];       // message from Android
  if(recvBT != oldBT)           // if a new message, then
  {                 //      transmit to receiving 
    if(recvBT.length() > 3) SerialBT.print(recvBT); //  Android tablet
    oldBT = recvBT;           // update message copy
  }
  LEDstate = jsonDoc["LED"];
  digitalWrite(LEDpin, LEDstate);
  if(LEDstate != oldLEDstate)
  {
    oldLEDstate = LEDstate;
    mesh.sendSingle(sender, recvMsg);
  }
}

void loop()
{
  mesh.update();
  if(SerialBT.available())        // new Bluetooth message
    sendBT = SerialBT.readString();     // read Bluetooth Serial buffer
}
