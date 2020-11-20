/*******************************************************************************
 * Sketch name: Semaphore method
 * Description: Allocating tasks to each ESP32 microcontroller core
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    22 - ESP32 microcontroller features
 ******************************************************************************/

int redLED = 26;
int blueLED= 27;
SemaphoreHandle_t baton;
volatile int redOff;

void setup()
{
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  xTaskCreatePinnedToCore(codeRed, "red LED ", 1000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(codeBlu, "blue LED", 1000, NULL, 1, NULL, 1);
  baton = xSemaphoreCreateMutex();
}

void codeRed(void * parameter)
{
  for (;;)
  {
    redOff = random(500, 2000);
    delay(1);
    xSemaphoreGive(baton);
    digitalWrite(redLED, HIGH);
    delay(1000);
    digitalWrite(redLED, LOW);
    delay(redOff);
  }
}

void codeBlu(void * parameter)
{
  for (;;)
  {
    xSemaphoreTake(baton, portMAX_DELAY);
    digitalWrite(blueLED, HIGH);
    delay(redOff);
    digitalWrite(blueLED, LOW);
  }
}

void loop()
{
  vTaskDelay(NULL);
}
