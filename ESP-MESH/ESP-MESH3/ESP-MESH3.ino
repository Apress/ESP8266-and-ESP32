/*******************************************************************************
 * Sketch name: ESP-MESH and Bluetooth communication (node 1)
 * Description: Transmission of text message, LED and relay states over ESP-MESH
 *              Bluetooth communication between ESP32 and app
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    ESP-MESH
 ******************************************************************************/
 #include <painlessMesh.h>        // differences from Listing 1
String ssid = "meshSSID";
String password = "meshPass";
int port = 5555;
int LEDstate = 0, relayState = 0;     // define LED and relay states
DynamicJsonDocument jsonDoc(256);     // reduce JsonDoc to 256

Scheduler scheduler;
painlessMesh mesh;
void sendMessage();
Task taskSend(TASK_SECOND * 1, TASK_FOREVER, &sendMessage);

#include <BluetoothSerial.h>        // include Bluetooth library
BluetoothSerial SerialBT;       // associate SerialBT with library
String sendBT, oldBT = "";        // Bluetooth message
char c;             // character for command letter

void setup()
{
  Serial.begin(115200);
  mesh.init(ssid, password, &scheduler, port);
  mesh.onReceive(&recvMessage);
  scheduler.addTask(taskSend);
  taskSend.enable();
  SerialBT.begin("ESP32 left");     // identify Bluetooth device
}

void sendMessage()
{
  jsonDoc["LED"] = LEDstate;        // update LED and relay states
  jsonDoc["relay"] = relayState;      // from Bluetooth buttons
  jsonDoc["msg"] = sendBT;        // message to Android tablet
  String sendMsg;
  serializeJson(jsonDoc, sendMsg);
  mesh.sendBroadcast(sendMsg);
  sendBT = "";            // reset message to Android tablet
  taskSend.setInterval((TASK_SECOND * 1));
}

void recvMessage(uint32_t sender, String &recvMsg)
{
  deserializeJson(jsonDoc, recvMsg);
  Serial.print("value ");         // display value on Serial
  Serial.println(jsonDoc["value"].as<long>());    //       plotter as an integer
  String recvBT = jsonDoc["msg"];       // message from Android
  if(recvBT != oldBT)           // if a new message, then
  {                 // transmit to receiving
    if(recvBT.length() > 3) SerialBT.print(recvBT); //  Android tablet
    oldBT = recvBT;           // update message copy
  }
}

void loop()
{
  mesh.update();
  if(SerialBT.available())          // new Bluetooth message
  {
    sendBT = SerialBT.readString();     // read Bluetooth Serial buffer
    c = sendBT[0];            // first letter of message
    switch (c)              // switch...case on letter
    {
    case 'L':             // letter = L (for LED)
      LEDstate = 1-LEDstate;          // alternate the LED state
      if(LEDstate == 1) SerialBT.println("LED ON"); // display LED state on
      else SerialBT.println("LED OFF");     // the Android tablet
      break;              // end of case
    case 'R':
      relayState = 1-relayState;        // similarly for the relay
      if(relayState == 1) SerialBT.println("relay ON");
      else SerialBT.println("relay OFF");
      break;
    default: break;           // no action, not L or R
    }
  }
}
