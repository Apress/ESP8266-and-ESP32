/*******************************************************************************
 * Sketch name: Rotary encoder switch
 * Description: Categorise length of switch press
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    19 - Rotary encoder
 ******************************************************************************/

int SWpin = D2;          // rotary encoder switch pin
int SWled = D6;         // LED pin
int longPress = 1000;       // time (ms) for long or short press
int shortPress = 500;
volatile unsigned long newTime, oldTime;
volatile char changeSW;       // variables in loop and isr functions
volatile unsigned int lagTime;

void setup()
{
  Serial.begin(115200);       // define Serial Monitor baud rate
  pinMode(SWled, OUTPUT);       // set LED pin as output
  pinMode(SWpin, INPUT_PULLUP);     // activate pull-up resistor
  attachInterrupt(digitalPinToInterrupt(SWpin), isr, CHANGE);
}             // SWpin as interrupt pin and attach isr

void loop()
{
  if(changeSW != ' ')         // switch released, turn on or off
  {               //  LED if long or short press
    Serial.print(lagTime);Serial.print("\t"); // display switch press time lag
    if(changeSW != 'B') digitalWrite(SWled, !digitalRead(SWled));
    if(changeSW == 'L') Serial.println("long press");
    else if(changeSW == 'S') Serial.println("short press");
    else Serial.println("switch bounce");   // display only, no effect on LED
    changeSW = ' ';         // reset change variable
  }
}

IRAM_ATTR void isr()
{
  newTime = millis();         // get time ISR triggered
  if(digitalRead(SWpin) == HIGH)      // switch now released
  {
    lagTime = newTime - oldTime;      // time between switch presses
    if(lagTime > longPress) changeSW = 'L';     // L for long press
    else if(lagTime > shortPress) changeSW = 'S';   // S for short press
    else changeSW = 'B';            // B for bounce
  }
  oldTime = newTime;          // reset switch press/release time
}
