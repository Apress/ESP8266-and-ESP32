/*******************************************************************************
 * Sketch name: Rotary encoder with square wave states
 * Description: Control LEDs by rotary encoder rotation direction
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    19 - Rotary encoder
 ******************************************************************************/

int CLKpin = D4;            // define rotary encoder pins
int DTpin = D3;
int redLED = D8;            // define LED pins
int greenLED = D7;
int vals[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; // array of scores
int count = 0;
volatile int score = 0;
volatile int change = 0;      // variables used in isr and loop functions
volatile int oldState = 0;

void setup()
{
  Serial.begin(115200);         // Serial Monitor baud rate
  pinMode(redLED, OUTPUT);        // set LED pins as output
  pinMode(greenLED, OUTPUT);        // attach interrupt
  pinMode(CLKpin, INPUT);       // required by ESP32
  pinMode(DTpin, INPUT);        // required by ESP32
  attachInterrupt(digitalPinToInterrupt(CLKpin), isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DTpin), isr, CHANGE);
}

void loop()
{
  if(change != 0)           // rotary encoder rotated
  {
    if(change > 0) LED(HIGH, "up   ");    // clockwise
    else if(change < 0) LED(LOW, "down ");  // anti-clockwise
    change = 0;
  }
}

void LED(int state, String text)
{
  digitalWrite(redLED, 1-state);      // turn LEDs on or off
  digitalWrite(greenLED, state);
  Serial.print(text);
  count = count + change;       // update count
  Serial.println(count);
}

IRAM_ATTR void isr()          // interrupt service routine
{               // construct 4-bit number
  int newState = (oldState<<2)+(digitalRead(CLKpin)<<1)+digitalRead(DTpin);
  score = score + vals[newState];     // allocate score from array
  oldState = newState % 4;    // remainder to leave new CLK and DT state
  if(score == 2 || score == -2)     // 2 steps for complete rotation
  {
    change = score/2;         // unit change per two steps
    score = 0;            // reset score
  }
}
