/*******************************************************************************
 * Sketch name: Capacitance meter
 * Description: Measure capacitor charging time
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    18 - Measuring electricity
 ******************************************************************************/

#include <Adafruit_SSD1306.h>        // Adafruit SSD1306 library
int width = 128;            // OLED screen size
int height = 32;            // associate oled with SSD1306
Adafruit_SSD1306 oled(width, height, &Wire, -1);
int capPin = A0;            // capacitor positive pin
int chargePin = D7;         // pin with 10k charge resistor
int dischargePin = D6;          // 220 discharge resistor pin
float resistor = 10000.0;       // 10k charge resistor
unsigned long startTime;
float mF, uF, nF;           // uF for microF (Greek letter )

void setup()
{
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // OLED display and I2C address
  oled.clearDisplay();          // clear OLED display
  oled.setTextColor(WHITE);         // set font color
  oled.setTextSize(2);          // set font size (1, 2, 3 or 4)
  oled.display();           // update display instructions
  pinMode(chargePin, OUTPUT);       // set charge pin
  digitalWrite(chargePin, LOW);     //  as OUTPUT and to 0V
}

void loop()
{
  oled.clearDisplay();
  oled.setCursor(0,0);
  digitalWrite(chargePin, HIGH);      // charge pin to reference voltage
  startTime = millis();         // start timing charging capacitor
  while(analogRead(capPin) < 648) {}    // do nothing while ADC < 648
  mF = (millis() - startTime) / resistor;   // calculate capacitance = time/R
  uF = 1000.0 * mF;         // change millifarad to microfarad
  if (uF > 1)
  {
    if (uF < 10) oled.print(uF, 1);     // display capacitance with 1DP
    else oled.print(uF, 0);       // or 0DP depending on value
    oled.print(" uF");
  }
  else
  {
    nF = 1000.0 * uF;         // convert to nanofarad
    if (nF > 10)            // only display if value > 10nF
    {
      oled.print(nF, 0);        // display capacitance
      oled.print(" nF");
    }
  }
  digitalWrite(chargePin, LOW);     // set charge pin to 0V
  pinMode(dischargePin, OUTPUT);      // set discharge pin
  digitalWrite(dischargePin, LOW);      //  to OUTPUT and 0V
  while(analogRead(capPin) > 0) {}    // do nothing while capacitor discharges
  pinMode(dischargePin, INPUT);     // set discharge pin to INPUT
  oled.display();
  delay(2000);            // to ensure no current flows
}
