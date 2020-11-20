/*******************************************************************************
 * Sketch name: PIR trigger to stream images to a webpage
 * Description: Images streamed to webpage for N seconds after triggered by PIR
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    2 - Intranet camera
 ******************************************************************************/
 
#include <esp_http_server.h>
#include <esp_camera.h>
#include <WiFi.h>
#include "config_pins.h"
#include "stream_handler.h"
#include <driver/rtc_io.h>        // include rtc input-output library
int PIRpin = 13;            // define PIR and LED pins
int LEDpin = 12;
unsigned long startTime, lastTime = 0;    // timer variables
int camTime = 10;           // define image streaming time (s)
int count = 0;            // counter for steaming time
char ssid[] = "xxxx";
char password[] = "xxxx";

void setup()
{
  pinMode(LEDpin, OUTPUT);        // LED pin as output
  flash();              // call function to flash LEDs
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  configCamera();
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_VGA);
  startCameraServer();
  rtc_gpio_pulldown_en((gpio_num_t)PIRpin); // pull-down PIR pin
  esp_sleep_enable_ext0_wakeup((gpio_num_t)PIRpin, 1);
}               // wakeup on PIR pin with state 1

void startCameraServer()
{
  httpd_handle_t stream_httpd = NULL;
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;
  httpd_uri_t index_uri = {.uri="/", .method=HTTP_GET,
                           .handler=stream_handler, .user_ctx=NULL};
  if (httpd_start(&stream_httpd, &config) == ESP_OK)
    httpd_register_uri_handler(stream_httpd, &index_uri);
}

void loop()
{
  if(count < 1) flash();        // call function to flash LEDs
  startTime = millis();         // start of image streaming time
  if(startTime - lastTime > 1000 && count < camTime)
  {               // display seconds elapsed
    Serial.print("camera ");Serial.println(count);
    count++;            // update counter
    lastTime = startTime;       // reset image streaming time
  }
  if(count == camTime)          // defined streaming time elapsed
  {
    flash();
    Serial.print("sleep mode on PIR pin ");Serial.println(PIRpin);
    esp_deep_sleep_start();       // ESP32 in sleep mode
  }
}

void flash()            // function to flash LEDs
{
  for (int i=0; i<3; i++)       // flash LED three times
  {
    digitalWrite(LEDpin, HIGH);     // turn on LED
    delay(200);
    digitalWrite(LEDpin, LOW);      // turn off LED
    delay(100);
  }
}
