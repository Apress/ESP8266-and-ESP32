/*******************************************************************************
 * Sketch name: Resistance meter (ohmmeter)
 * Description: Voltage divider to determine resistor value
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    18 - Measuring electricity
 ******************************************************************************/

#include <Adafruit_SSD1306.h>        // Adafruit SSD1306 library
int width = 128;            // OLED screen size
int height = 32;            // associate oled with SSD1306
Adafruit_SSD1306 oled(width, height, &Wire, -1);
int resistPin = A0;         // analog input pin
int pin[] = {D5, D6, D7};       // pins for known resistor
float known[] = {1000.0, 4700.0, 10000.0};  // known resistor values
float resist, reading;

void setup()
{
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // OLED display and I2C address
  oled.clearDisplay();          // clear OLED display
  oled.setTextColor(WHITE);       // set font color
  oled.setTextSize(1);          // set font size (1, 2, 3 or 4)
  oled.display();           // update display instructions
// set known resistor pins to INPUT
  for (int i=0; i<3; i++) pinMode(pin[i], INPUT);
}

void loop()
{
  oled.clearDisplay();          // clear OLED display
  oled.setCursor(0,0);
  oled.print("known   ADC  predict");   // header on OLED screen
  for (int i=0; i<3; i++)       // for each known resistor
  {
    pinMode(pin[i], OUTPUT);        // set known resistor pin
    digitalWrite(pin[i], HIGH);     //  to OUTPUT and to HIGH
    reading = analogRead(resistPin);    // voltage divider reading
    if(reading < 10 || reading > 1013) reading = 0; // constrain ADC reading
    resist = known[i]*reading/(1024.0-reading); // calculate resistance
    pinMode(pin[i], INPUT);        // reset known resistor pin to INPUT
    oled.setCursor(1, (i+1)*8);     // OLED column 1, row 8, 16, 24
    oled.print(known[i],0);       // display known resistor
    oled.setCursor(50, (i+1)*8);
    oled.print(reading,0);        // display ADC reading
    oled.setCursor(90, (i+1)*8);
    oled.print(resist,0);       // display calculated resistance
    oled.display();
  }
  delay(5000);
}
