/*******************************************************************************
 * Sketch name: Control incremental count by rotary encoder rotation speed
 * Description: Incremental change depends on rotation speed
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    19 - Rotary encoder
 ******************************************************************************/

int CLKpin = D4;
int DTpin = D3;
int redLED = D8;
int greenLED = D7;
int vals[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
int count = 0;
volatile int score = 0; volatile int change = 0;
volatile int oldState = 0;
int incLED = D6;            // increment indicator LED pin
int rotation = 500;         // threshold rotation time (ms)
int increment = 1;          // default increment
volatile unsigned long newTime, oldTime, lagTime;
volatile int newCLK;          // variables used in ISR
volatile int oldCLK = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(CLKpin, INPUT);
  pinMode(DTpin, INPUT);
  attachInterrupt(digitalPinToInterrupt(CLKpin), isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DTpin), isr, CHANGE);
  pinMode(incLED, OUTPUT);        // increment indicator LED
}

void loop()
{
  if(change != 0)
  {
    if(change > 0) LED(HIGH, "up   ");
    else if(change < 0) LED(LOW, "down ");
    change = 0;
  }
}

void LED(int state, String text)
{
  digitalWrite(redLED, 1-state);
  digitalWrite(greenLED, state);
  Serial.print(text);
  count = count + change*increment;     // incremented count
  digitalWrite(incLED, increment>1);    // turn LED on, increment = 10
  Serial.print(increment);Serial.print("\t"); // display updated increment
  Serial.println(count);
}

IRAM_ATTR void isr()
{               // avoid re-reading the square
  newCLK = digitalRead(CLKpin);     // wave state of pin A (CLK)
  int newState = (oldState<<2)+(newCLK<<1)+digitalRead(DTpin);
  score = score + vals[newState];
  oldState = newState % 4;
  if(score == 2 || score == -2)
  {
    change = score/2;
    score = 0;
  }
  newTime = millis();         // get time ISR triggered
  if(newCLK == HIGH && oldCLK == LOW)   // interval between falling
  {               // and rising edge on pin A (CLK)
    lagTime = newTime - oldTime;
    if(lagTime < rotation && lagTime > 100) increment = 10; // fast rotation
    else if(lagTime > rotation) increment = 1;      // slow rotation
    oldTime = newTime;          // reset rising/falling edge time
  }
  oldCLK = newCLK;          // reset state of pin A (CLK)
}
