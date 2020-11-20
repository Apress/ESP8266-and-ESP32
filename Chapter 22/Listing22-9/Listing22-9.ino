/*******************************************************************************
 * Sketch name: RTC timer and sleep mode
 * Description: Activate ESP32 microcontroller from sleep mode after time interval
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    22 - ESP32 microcontroller features
 ******************************************************************************/

RTC_DATA_ATTR int count = 0;        // store count in RTC memory
unsigned long micro = 5000000;      // time interval in s

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  esp_sleep_enable_timer_wakeup(micro);   // RTC timer interval in s
}

void loop()
{
  count++;              // increment and print count
  Serial.print("count ");Serial.println(count);
  esp_deep_sleep_start();       // microcontroller in sleep mode
}
