/*******************************************************************************
 * Sketch name: Battery voltage measurement
 * Description: Display voltage and battery figure on OLED screen
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    18 - Measuring electricity
 ******************************************************************************/

#include <Adafruit_SSD1306.h>        // Adafruit SSD1306 library
int width = 128;            // OLED screen size
int height = 32;            // associate oled with SSD1306
Adafruit_SSD1306 oled(width, height, &Wire, -1);
int ADCpin = A0;            // define analog input pin
float maxVolt = 10.24;          // maximum battery voltage
float voltage;
int battery;

void setup()
{
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // OLED display and I2C address
  oled.clearDisplay();          // clear OLED display
  oled.setTextColor(WHITE);       // set font color
  oled.setTextSize(2);          // set font size (1, 2, 3 or 4)
  oled.display();           // start display instructions
}

void loop()
{
  voltage = analogRead(ADCpin)*maxVolt/1024.0;  // calculate battery voltage
  oled.clearDisplay();
  oled.setCursor(0,0);          // position cursor at (0, 0)
  oled.print(voltage);
  oled.print("V");          // display battery voltage
  oled.fillRect(0, 20, 45, 12, WHITE);    // battery frame 2 pixels width 
  battery = 41*voltage/maxVolt;     // full battery section
  battery = constrain(battery, 0, 41);
  oled.fillRect(2+battery, 22, 41-battery, 8, BLACK); // empty battery section
  oled.fillRect(45, 23, 3, 6, WHITE);   // battery top
  oled.display();
  delay(2000);            // delay 2s between readings
}
