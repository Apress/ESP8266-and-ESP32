/*******************************************************************************
 * Sketch name: Ammeter with shunt resistor
 * Description: Display current on OLED screen
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    18 - Measuring electricity
 ******************************************************************************/

#include <Adafruit_SSD1306.h>        // Adafruit SSD1306 library
int width = 128;            // OLED screen size
int height = 32;            // associate oled with SSD1306
Adafruit_SSD1306 oled(width, height, &Wire, -1);
int ADCpin = A0;
unsigned long ADC;
int Rfeedback = 100000;         // feedback resistor value
int RGND = 2200;            // resistor GND value
float current, opAmpmV, shuntmV, gain;

void setup()
{
  Serial.begin(115200);
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // OLED display and I2C address
  oled.clearDisplay();          // clear OLED display
  oled.setTextColor(WHITE);       // set font color
  oled.setTextSize(1);          // set font size (1, 2, 3 or 4)
  oled.display();           // update display instructions
  gain = 1.0 + Rfeedback/RGND;      // calculate op amp gain
}

void loop()
{
  ADC = 0;
  for (int i=0; i<100; i++)       // repeated analog readings
  {
    ADC = ADC + analogRead(ADCpin);
    delay(10);
  }
  ADC = ADC/100;            // average of analog readings
  opAmpmV = ADC*3200.0/1024;        // op amp output voltage
  shuntmV = opAmpmV / gain;       // voltage on shunt
  current = 100.0 * shuntmV;        // shunt mV to current in mA
  oled.clearDisplay();
  oled.setCursor(0,0);          // display results on OLED
  oled.print("current  ");oled.print(current,0); oled.println(" mA");
  oled.print("shunt mV ");oled.println(shuntmV,0);
  oled.print("opamp mV ");oled.println(opAmpmV,0);
  oled.print("ADC      ");oled.println(ADC);
  oled.display();
  delay(1000);
}
