/*******************************************************************************
 * Sketch name: TFT-eSPI library User_Setup settings for ESP8266 and ESP32 development boards
 * Description: Pin numbers for ESP8266 or ESP32 - required by TFT-eSPI library
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    3 - International weather station
 ******************************************************************************/

#define TFT_CS   PIN_D8          // ESP8266 SPI and touch screen
#define TFT_DC   PIN_D4
#define TFT_RST  PIN_D0 
#define TOUCH_CS PIN_D1

/*      // lines between /* and */ are commented out
#define TFT_MISO 19         // ESP32 SPI and touch screen
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS    5
#define TFT_DC   26
#define TFT_RST  25
#define TOUCH_CS 27
*/

#define ILI9341_DRIVER          // ILI9341 SPI TFT LCD screen
#define TFT_RGB_ORDER TFT_BGR       // color order Blue-Green-Red
#define LOAD_GLCD           // font 1: Adafruit 8-pixel high
#define LOAD_FONT2          // font 2: small 16-pixel high
#define LOAD_FONT4          // font 4: medium 26-pixel high
#define SPI_FREQUENCY 40000000      // SPI 40MHz
#define SPI_READ_FREQUENCY 20000000     // SPI read 20MHz
#define SPI_TOUCH_FREQUENCY 2500000     // SPI touch 2.5MHz
