/*******************************************************************************
 * Sketch name: Generating a sine wave with R-2R ladder 8-bit DAC
 * Description: generate sine waves
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    16 - Signal generation
 ******************************************************************************/

int dataPin = D5;            // shift register data
int latchPin = D6;          // latch and clock pins
int clockPin = D7;
int count = 0;
float sum, angle, Hz;
unsigned long lastTime;
int val, sign, mult, cycleTime;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  pinMode(dataPin, OUTPUT);       // shift register pins as output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void loop()
{
  mult = 20.0*analogRead(A0)/1023.0;    // scale potentiometer value
  for (int deg=0; deg<360; deg=deg+mult)      // cycle through 360°
  {
    angle = deg*PI/180.0;       // convert degrees to radians
    sum = sin(angle);
    val = round(128+120.0*sum);     // scaled sine wave value
    digitalWrite(latchPin, LOW);      // start loading shift register
    shiftOut(dataPin, clockPin, MSBFIRST, val);   // load 8-bit number
    digitalWrite(latchPin, HIGH);     // end loading shift register
  }
  count ++;
  if(count > 999)           // display every 1000 cycles
  {
    cycleTime = millis() - lastTime;    // time (ms) for cycle
    Hz = 1000.0 * count / cycleTime;    // frequency (Hz)
    Serial.print(mult);Serial.print("\t");
    Serial.print(Hz);Serial.print("Hz\t");
    Serial.print(1.0*cycleTime/count);Serial.println("ms");
    count = 0;            // reset counter
    lastTime = millis();        // reset timer
  }
}
