/*******************************************************************************
 * Sketch name: Rotary encoder with interrupt
 * Description: Control LEDs by rotary encoder rotation direction
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    19 - Rotary encoder
 ******************************************************************************/

int CLKpin = D4;
int DTpin = D3;
int redLED = D8;
int greenLED = D7;
int count = 0;
volatile int change;            // variable used in ISR

void setup()
{
  Serial.begin(115200);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);          // attached interrupt
  attachInterrupt(digitalPinToInterrupt(CLKpin), isr, FALLING);
}

void loop()
{
  if(change != 0)             // rotary encoder direction
  {
    if(change > 0) LED(HIGH, "up   ");      // clockwise
    else if(change < 0) LED(LOW, "down ");    // anti-clockwise
    change = 0;
  }
}

void LED(int state, String text)      // no change to LED function
{
  digitalWrite(redLED, 1-state);
  digitalWrite(greenLED, state);
  Serial.print(text);
  count = count + change;
  Serial.println(count);
}

IRAM_ATTR void isr()            // interrupt service routine
{
  change = 2*digitalRead(DTpin) - 1;    // determine direction of rotation
}
