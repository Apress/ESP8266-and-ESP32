/*******************************************************************************
 * Sketch name: Queue method
 * Description: Allocating tasks to each ESP32 microcontroller core
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    22 - ESP32 microcontroller features
 ******************************************************************************/

int redLED = 26;
int blueLED = 27;
QueueHandle_t queue;

void setup()
{
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  xTaskCreatePinnedToCore(codeRed, "red LED ", 1000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(codeBlue, "blue LED", 1000, NULL, 1, NULL, 1);
  queue = xQueueCreate(1, sizeof(int));
}

void codeRed(void * parameter)
{
  for (;;)
  {
    int redOff = random(500, 2000);
    delay(1);
    xQueueSend(queue, &redOff, portMAX_DELAY);
    digitalWrite(redLED, HIGH);
    delay(1000);
    digitalWrite(redLED, LOW);
    delay(redOff);
  }
}

void codeBlue(void * parameter)
{
  for (;;)
  {
    int blueOn;
    xQueueReceive(queue, &blueOn, portMAX_DELAY);
    digitalWrite(blueLED, HIGH);
    delay(blueOn);
    digitalWrite(blueLED, LOW);
  }  
}
void loop()
{
  vTaskDelay(NULL);
}
