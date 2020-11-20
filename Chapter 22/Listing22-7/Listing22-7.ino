/*******************************************************************************
 * Sketch name: Timer control of two independent events
 * Description: Simultaneous control of two events
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    22 - ESP32 microcontroller features
 ******************************************************************************/

hw_timer_t * timer1 = NULL;          // define timer1
portMUX_TYPE timer1Mux = portMUX_INITIALIZER_UNLOCKED;
hw_timer_t * timer2 = NULL;           // define timer2
portMUX_TYPE timer2Mux = portMUX_INITIALIZER_UNLOCKED;
int LED1pin = 25;
int LED2pin = 26;             // define LED pins

void setup()
{
  Serial.begin(115200);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);
  timer1 = timerBegin(1, 80, true);       // set timer1 properties
  timerAttachInterrupt(timer1, &timer1ISR, true);
  timerAlarmWrite(timer1, 250000, true);      // interval of 0.25s
  timerAlarmEnable(timer1);
  timer2 = timerBegin(2, 80, true);       // set timer2 properties
  timerAttachInterrupt(timer2, &timer2ISR, true);
  timerAlarmWrite(timer2, 1000000, true);     // interval of 1s
  timerAlarmEnable(timer2);
}

void loop()
{
  vTaskDelay(NULL);
}

IRAM_ATTR void timer1ISR()          // ISR for timer1
{
  portENTER_CRITICAL_ISR(&timer1Mux);
  digitalWrite(LED1pin, !digitalRead(LED1pin));
  portEXIT_CRITICAL_ISR(&timer1Mux);
}

IRAM_ATTR void timer2ISR()          // ISR for timer 2
{
  portENTER_CRITICAL_ISR(&timer2Mux);
  digitalWrite(LED2pin, !digitalRead(LED2pin));
  portEXIT_CRITICAL_ISR(&timer2Mux);
}
