/*******************************************************************************
 * Sketch name: R-2R digital to analog converter
 * Description: R-2R resistor ladder 8-bit DAC
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    16 - Signal generation
 ******************************************************************************/

int dataPin = D5;            // shift register data
int latchPin = D6;          // latch and clock pins
int clockPin = D7;
int Vin;              // voltage on analog pin
float voltage, predict;         // voltage divider effect and
float voltDivid = 1000.0*(10+220+100)/100;  // adjustment to mV

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  pinMode(dataPin, OUTPUT);       // shift register pins as output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void loop()             // nothing in loop function
{
  for (int i=0; i<256; i=i+50)      // incremental increases
  {
    digitalWrite(latchPin, LOW);      // start loading shift register
    shiftOut(dataPin, clockPin, MSBFIRST, i); // load 8-bit number
    digitalWrite(latchPin, HIGH);     // end loading shift register
    Vin = analogRead(A0);       // read R-2R ladder voltage
    voltage = Vin * voltDivid / 1024.0;   // scaled R-2R ladder voltage
    predict = i * 3300.0 / 256;     // predicted output voltage
    Serial.print(i);Serial.print("\t");
    Serial.print(Vin);Serial.print("\t");   // display results
    Serial.print(voltage,0);Serial.print("\t");
    Serial.println(predict,0);
    delay(200);
  }
}
