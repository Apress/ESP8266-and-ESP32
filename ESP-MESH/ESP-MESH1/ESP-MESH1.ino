/*******************************************************************************
 * Sketch name: Microcontroller with switches and OLED (node 1)
 * Description: Transmission of LED and relay states over ESP-MESH network
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    ESP-MESH
 ******************************************************************************/
#include <painlessMesh.h>        // include painlessMesh library
String ssid = "meshSSID";       // ESP-MESH name & password
String password = "meshPass";
int port = 5555;            // ESP-MESH port
int LEDswitchPin = D3;
int relaySwitchPin = D4;        // LED and relay pins
int LEDstate, relayState;
DynamicJsonDocument jsonDoc(1024);      // JSON document

#include <Adafruit_SSD1306.h>     // include Adafruit SSD1306 library
int width = 128;            // OLED width and height
int height = 32;
Adafruit_SSD1306 oled(width, height, &Wire, -1);

Scheduler scheduler;          // associate scheduler with library
painlessMesh mesh;          // associate mesh with library
void sendMessage();
Task taskSend(TASK_SECOND * 1, TASK_FOREVER, &sendMessage); // task timing of 1s

void setup()
{
  Serial.begin(115200);
  mesh.setDebugMsgTypes(ERROR | STARTUP);     // before init instruction
  mesh.init(ssid, password, &scheduler, port);
  mesh.onReceive(&recvMessage);          // set recvMessage function
  mesh.onNewConnection(&newConnectionCallback);   // ESP-MESH functions
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  scheduler.addTask(taskSend);        // schedule and enable
  taskSend.enable();            // send message function

  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();            // clear OLED display
  oled.setTextSize(1);            // characters 68 pixels
  oled.setTextColor(WHITE);
  oled.display();             // update display
}

void sendMessage()          // function to send message
{                 // change LED state
  if (digitalRead(LEDswitchPin) == LOW) LEDstate = 1-LEDstate;
  jsonDoc["LED"] = LEDstate;          // update JSON document
  if (digitalRead(relaySwitchPin) == LOW) relayState = 1-relayState;
  jsonDoc["relay"] = relayState;
  String sendMsg;           // sendMsg defined in function
  serializeJson(jsonDoc, sendMsg);        // convert to message
  mesh.sendBroadcast(sendMsg);        // broadcast message
  taskSend.setInterval((TASK_SECOND * 1));    // message timing of 1s
}

void recvMessage(uint32_t sender, String &recvMsg)    // function to receive message
{
  Serial.print("from ");Serial.print(sender);   // sender identity
  Serial.print("\t");Serial.println(recvMsg);   // Serial display message
  DeserializationError error = deserializeJson(jsonDoc, recvMsg);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
  }
  oled.clearDisplay();            // update OLED screen
  oled.setCursor(0,0);
  oled.print("LED ");           // LED as ON or OFF
  if(jsonDoc["LED"].as<long>() == 1) oled.print("ON");
  else oled.print("OFF");
  oled.setCursor(0,8);
  oled.print("relay ");           // relay as ON or OFF
  if(jsonDoc["relay"].as<long>() == 1) oled.print("ON");
  else oled.print("OFF");
  oled.setCursor(0,16);           // generated sensor data
  oled.print("value ");oled.print(jsonDoc["value"].as<long>());
  oled.display();
}

void newConnectionCallback(uint32_t nodeID)   // new node connected
{                 // display node identity
  Serial.print("New connection nodeID ");Serial.println(nodeID);
}

void changedConnectionCallback()      // function to list connected nodes
{
  SimpleList<uint32_t> list = mesh.getNodeList();   // number of nodes
  Serial.print("number of nodes ");Serial.println(list.size());
  Serial.println("Connection list");
  Serial.print("node1 ");Serial.println(mesh.getNodeId());  // node 1 identity
  SimpleList<uint32_t>::iterator node = list.begin();
  while (node != list.end())            // list of nodes
  {
    Serial.print("nodeID ");Serial.println(*node);    // *node is identity
    node++;                 // increment node
  }
}

void nodeTimeAdjustedCallback(int32_t offset) // display synchronised timing
{
  Serial.printf("adjust %u offset = %d\n", mesh.getNodeTime(), offset);
}

void loop()
{
  mesh.update();              // handle ESP-MESH
}
