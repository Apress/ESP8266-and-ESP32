/*******************************************************************************
 * Sketch name: Inductor meter
 * Description: Measure oscillation frequency after voltage pulse
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    18 - Measuring electricity
 ******************************************************************************/

#include <Adafruit_SSD1306.h>        // Adafruit SSD1306 library
int width = 128;            // OLED screen size
int height = 32;            // associate oled with SSD1306
Adafruit_SSD1306 oled(width, height, &Wire, -1);
int voltPin = D0;           // voltage burst pin
int LM393pin = D3;          // LM393 output pin
float capacitor = 1.0;          // measured in F
int timeout = 1000;         // timeout limit in s
float pulse, pulse1, pulse2, frequency, inductance;

void setup()
{
  pinMode(voltPin, OUTPUT);       // define voltPin as output
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // OLED I2C address
  oled.clearDisplay();          // clear OLED display
  oled.setTextColor(WHITE);       // set font color
  oled.setTextSize(1);          // set font size (1, 2, 3 or 4)
  oled.display();           // update display instructions

}
void loop()
{
  digitalWrite(voltPin, HIGH);      // apply 3.2V on voltPin
  delay(5);             // time to charge the inductor
  digitalWrite(voltPin,LOW);        // end of voltage burst
  pulse1 = pulseIn(LM393pin, HIGH, timeout);  // measure HIGH pulse duration
  pulse2 = pulseIn(LM393pin, HIGH, timeout);
  if(pulse2 > 0) pulse = (pulse1+pulse2)/2.0; // average pulse length
  else pulse = pulse1;
  if(pulse > 0)
  {
    frequency = 1E6/(2.0*pulse);      // shorthand for 10 to the power 6
    inductance = pulse*pulse/(PI*PI*capacitor); // calculate inductance
    oled.clearDisplay();
    oled.setCursor(0,0);        // display results
    oled.print("inductance uH ");oled.println(inductance,0);
    oled.print("frequency Hz  ");oled.println(frequency,0);
    oled.print("high time us  ");oled.print(pulse,0);
    oled.display();
  }
  delay(1000);
}
