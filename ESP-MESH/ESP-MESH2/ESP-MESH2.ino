/*******************************************************************************
 * Sketch name: Microcontroller with LED and generated data (node 3)
 * Description: Reception of LED state over ESP-MESH with callback
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    ESP-MESH
 ******************************************************************************/
 #include <painlessMesh.h>        // differences from Listing 1
String ssid = "meshSSID";
String password = "meshPass";
int port = 5555;
int LEDpin = D2;              // define LED pin
int LEDstate, oldLEDstate;          // new and old LED states
DynamicJsonDocument jsonDoc(1024);

Scheduler scheduler;
painlessMesh mesh;
void sendMessage();
Task taskSend(TASK_SECOND * 1 , TASK_FOREVER, &sendMessage);

void setup()
{
  Serial.begin(115200);
  pinMode(LEDpin, OUTPUT);          // LED pin as OUTPUT
  mesh.init(ssid, password, &scheduler, port);
  mesh.onReceive(&recvMessage);
  scheduler.addTask(taskSend);
  taskSend.enable();
}

void sendMessage()          // function to send message
{
  float value = random(0, 100);     // generate random number
  jsonDoc["value"] = value;       // update name and value pair
  String sendMsg;
  serializeJson(jsonDoc, sendMsg);
  mesh.sendBroadcast(sendMsg);
  taskSend.setInterval((TASK_SECOND * 3));    // message timing of 3s
}

void recvMessage(uint32_t sender, String &recvMsg)
{
  deserializeJson(jsonDoc, recvMsg);
  LEDstate = jsonDoc["LED"];          // update LED state
  digitalWrite(LEDpin, LEDstate);       // and LED pin state
  if(LEDstate != oldLEDstate)         // turn on or off  LED
  {                 // if LED state changed
    oldLEDstate = LEDstate;         // update old LED state
    mesh.sendSingle(sender, recvMsg);     // send message to node 1
  }
}

void loop()
{
  mesh.update();
}
