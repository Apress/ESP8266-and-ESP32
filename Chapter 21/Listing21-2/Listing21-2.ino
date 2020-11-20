/*******************************************************************************
 * Sketch name: Microcontroller information
 * Description: Display SPI, I2C pin numbers and microcontroller details 
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    21 - Microcontrollers
 ******************************************************************************/

String str, adjStr;

void setup()
{
  Serial.begin(115200);          // Serial Monitor baud rate
  Serial.println();
  while(!Serial);           // Pro Micro wait for serial port
  Serial.print("Arduino IDE version ");
  str = String(ARDUINO);        // Arduino IDE version
  adjStr = str.substring(1,str.length()-5)+".";
  adjStr = adjStr + str.substring(str.length()-4, str.length()-2)+".";
  adjStr = adjStr + str.substring(str.length()-2);
  Serial.println(adjStr);       // date and time sketch compiled
  Serial.print("Compiler version    "); Serial.println(__VERSION__);
  Serial.print("Compiled date       "); Serial.println(__DATE__);
  Serial.print("Compiled time       "); Serial.println(__TIME__);
  Serial.print("Sketch location     "); Serial.println(__FILE__);
  Serial.print("CPU frequency(MHz)  ");       // CPU frequency
  Serial.println(F_CPU/1000000);
  Serial.print("Development board   ");Serial.println(ARDUINO_BOARD);
  #ifdef __AVR__                  // development board
    Serial.print("Microcontroller     ");Serial.println(ARDUINO_MCU);
  #endif                  // microcontroller
  Serial.print("SPI MOSI ");Serial.println(MOSI);   // pin layout SPI
  Serial.print("SPI MISO ");Serial.println(MISO);
  Serial.print("SPI SCK  ");Serial.println(SCK);
  Serial.print("SPI SS   ");Serial.println(SS);
  Serial.print("I2C SDA  ");Serial.println(SDA);    // pin layout I2C
  Serial.print("I2C SCL  ");Serial.println(SCL);
#ifndef ESP32
  Serial.print("LED      ");Serial.println(LED_BUILTIN);  // built-in LED
#endif
}

void loop()             // nothing in loop function
{}
