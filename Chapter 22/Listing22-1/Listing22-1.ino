/*******************************************************************************
 * Sketch name: A task on each core
 * Description: Allocating tasks to each ESP32 microcontroller core
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    22 - ESP32 microcontroller features
 ******************************************************************************/

int redLED = 26;            // define LED pins
int blueLED = 27;
TickType_t xOneSec;         // create time delay variable

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  pinMode(redLED, OUTPUT);        // set LED pins as output
  pinMode(blueLED, OUTPUT);
  xOneSec = 1000 / portTICK_PERIOD_MS;    // define number of ticks
  xTaskCreatePinnedToCore(codeRed, "red LED one sec", // allocate tasks to cores
1000, NULL, 2, NULL, 0);
  xTaskCreatePinnedToCore(codeBlue, "blue LED quarter sec", 
1000, NULL, 1, NULL, 1);
}

void codeRed(void * parameter)      // function for red LED
{
  for (;;)
  {
    int tickTime = xTaskGetTickCount();   // tick count at start
    digitalWrite(redLED, HIGH);     // turn on or off LED
    vTaskDelay(xOneSec);        // task delay for one second
    digitalWrite(redLED, LOW);
    vTaskDelay(xOneSec);
    int tick = xTaskGetTickCount() - tickTime;  // change in tick count
    Serial.print("Core ");Serial.print(xPortGetCoreID());
    Serial.print(" red ");Serial.println(tick);
  }
}

void codeBlue(void * parameter)     // similar task with 250ms delay
{
  for (;;)
  {
    unsigned long start = millis();     // time at start
    digitalWrite(blueLED, HIGH);
    delay(250);           // task delay of 250ms
    digitalWrite(blueLED, LOW);
    delay(250);
    start = millis() - start;
    Serial.print("Core ");Serial.print(xPortGetCoreID());
    Serial.print(" blue ");Serial.println(start);
  }
}

void loop()
{               // no instructions in loop function
  vTaskDelay(NULL);         // other than zero delay
} 
