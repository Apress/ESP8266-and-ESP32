/*******************************************************************************
 * Sketch name: Rotary encoder with LEDs
 * Description: Control LEDs by rotary encoder rotation direction
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    19 - Rotary encoder
 ******************************************************************************/

int CLKpin = D4;            // rotary encoder pins
int DTpin = D3;           // CLK = pin A and DT = pin B
int redLED = D8;
int greenLED = D7;          // define LED pins
int count = 0;
int oldCLK = LOW;
int newCLK;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  pinMode(redLED, OUTPUT);        // set LED pins as output
  pinMode(greenLED, OUTPUT);
}

void loop()
{
  newCLK = digitalRead(CLKpin);     // state of pin A (CLK)
  if(newCLK == LOW && oldCLK == HIGH)   // falling edge of pin A
  {
    if(digitalRead(DTpin) == HIGH) LED(HIGH, 1, "up   "); // pin B HIGH
    else LED(LOW, -1, "down ");     // pin B (DT) LOW
  }
  oldCLK = newCLK;          // reset pin A (CLK) state
//  delay(100);           // to confirm microcontroller
}               // can miss rotary encoder turns

void LED(int state, int increment, String text)
{
  digitalWrite(redLED, 1-state);      // turn on or off the LEDs
  digitalWrite(greenLED, state);
  Serial.print(text);
  count = count + increment;        // update and display count
  Serial.println(count);
}
