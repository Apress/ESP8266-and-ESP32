/*******************************************************************************
 * Sketch name: Current sensor MAX471 and ESP8266 development board
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
int currentPin = A0;          // analog input pin
float current;

void setup()
{
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // OLED I2C address
  oled.clearDisplay();          // clear OLED display
  oled.setTextColor(WHITE);       // set font color
  oled.setTextSize(3);          // set font size (1, 2, 3 or 4)
  oled.display();           // update display instructions
}

void loop()
{  
  current = analogRead(currentPin);     // analog MAX471 reading
  current = 1000*current*3.2/1024;      // convert to current
  oled.clearDisplay();
  oled.setCursor(0,0);          // position cursor
  oled.print(current,0); oled.print(" mA"); // display current
  oled.display(); 
  delay(5000);
}
