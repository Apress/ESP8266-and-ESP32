/*******************************************************************************
 * Sketch name: Current sensor (INA219)
 * Description: Display current on OLED screen
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    18 - Measuring electricity
 ******************************************************************************/

#include <Adafruit_INA219.h>        // define Adafruit_INA219 lib
Adafruit_INA219 ina219;       // default I2C, associate ina219 with lib
#include <Adafruit_SSD1306.h>       // Adafruit SSD1306 library
int width = 128;            // OLED screen size
int height = 32;            // associate oled with SSD1306
Adafruit_SSD1306 oled(width, height, &Wire, -1);
float shunt, load, supply, current, power;
float energy = 0;

void setup()
{
  ina219.begin();
//  ina219.setCalibration_32V_2A();     // default precision option
//  ina219.setCalibration_32V_1A();     // intermediate option
  ina219.setCalibration_16V_400mA();    // high precision option
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // OLED I2C address
  oled.clearDisplay();          // clear OLED display
  oled.setTextColor(WHITE);       // set font color
  oled.setTextSize(1);          // set font size (1, 2, 3 or 4)
  oled.display();           // update display instructions
}

void loop() 
{
  shunt = ina219.getShuntVoltage_mV();    // shunt voltage in mV
  load = ina219.getBusVoltage_V();      // load voltage in V
  supply = load + shunt / 1000.0;     // supply voltage in V
  current = ina219.getCurrent_mA();     // current in mA
  power = ina219.getPower_mW();     // power in mW
  energy = energy + power / 3600.0;     // energy in mAh
  oled.clearDisplay();
  oled.setCursor(0,0);          // display results
  oled.print("current ");oled.print(current,0); oled.println(" mA");
  oled.print("shunt   ");oled.print(shunt,0); oled.println(" mV");
  oled.print("power   ");oled.print(power,0); oled.println(" mW");
  oled.print("energy  ");oled.print(energy,0); oled.print(" mAh"); 
  oled.display();
  delay(5000);
  oled.clearDisplay();
  oled.setCursor(0,0);          // display supply and load V
  oled.print("supply ");oled.print(supply); oled.println(" V");
  oled.print("load   ");oled.print(load); oled.println(" V");
  oled.display();
  delay(5000);
}
