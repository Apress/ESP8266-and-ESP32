/*******************************************************************************
 * Sketch name: Battery voltage with and without load measurement
 * Description: Display voltages on OLED screen
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    18 - Measuring electricity
 ******************************************************************************/

#include <Adafruit_SSD1306.h>        // SSD1306 library for OLED
int width = 128;            // OLED screen size
int height = 32;            // associate oled with SSD1306
Adafruit_SSD1306 oled(width, height, &Wire, -1);
int basePin = D6;           // BJT base pin
int collPin = A0;           // BJT collector pin
unsigned long sum;
int reps = 10;            // number of repeat measurements
float maxVolt = 3.2;          // 3.2V maximum voltage
float Vbatt, Vload;

void setup()
{
  pinMode(basePin, OUTPUT);       // define basePin as OUTPUT
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // OLED I2C address
  oled.clearDisplay();          // clear OLED display
  oled.setTextColor(WHITE);       // OLED font color
  oled.setTextSize(1);          // font size 1 character 68 pixels
  oled.display();           // start display instructions
}

void loop()
{
  readings(LOW);            // read BJT collector pin, BJT off
  Vbatt = maxVolt*sum/(reps*1024.0);    // battery voltage
  if(Vbatt > 0.1)
  {
    readings(HIGH);         // read BJT collector pin, BJT on
    Vload = maxVolt*sum/(reps*1024.0);    // battery voltage with load
    screen();           // call screen function
  }
  else
  {
    oled.clearDisplay();        // clear OLED when
    oled.display();         // no battery voltage
  }
  delay(2000);            // delay between measurements
}

void readings(int pinState)       // function to measure BJT pin
{
  digitalWrite(basePin, pinState);      // BJT base pin turned on or off
  sum = 0;
  for (int i=0; i<reps; i++)        // repeat voltage measurements
  {
    sum = sum + analogRead(collPin);    // sum of voltage measurements
    delay(5);
  }
  digitalWrite(basePin, LOW);       // turn off BJT base pin
}

void screen()           // function for OLED display
{
  oled.clearDisplay();          // clear OLED display
  oled.setCursor(0,0);          // move cursor to position (0,0)
  oled.print("battery ");oled.print(Vbatt,3);oled.println("V");
  oled.setCursor(0, 12);
  oled.print("+load   ");oled.print(Vload,3);oled.print("V");
  oled.setCursor(0, 24);
  oled.print("perform ");oled.print(100.0*Vload/Vbatt,0);oled.print("%");
  oled.display();
}
