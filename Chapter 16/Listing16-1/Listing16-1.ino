/*******************************************************************************
 * Sketch name: AD9833 sine and triangle wave generator
 * Description: Generate waves with MD-AD9833 library
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    16 - Signal generation
 ******************************************************************************/

#include <SPI.h>            // include SPI library
#include <MD_AD9833.h>          // include MD-AD9833 library
int FSYNC = D0;           // define data synchronisation pin
MD_AD9833 AD(FSYNC);
MD_AD9833::channel_t chan;        // library channel variable
MD_AD9833::mode_t mode;         // library signal mode variable
unsigned long freq;

void setup()
{
  AD.begin();           // initialise library
  chan = MD_AD9833::CHAN_0;       // set channel as 0 or 1
  AD.setActiveFrequency(chan);      // activate signal generator
}

void loop() 
{
  wave('s', 30000, 5000);       // call sine and triangle
  wave('t', 20000, 5000);       //  wave display function
}

void wave(char shape, unsigned long freq, int timeint)
{
  if(shape == 's') mode = MD_AD9833::MODE_SINE;     // sine wave
  else if(shape == 't') mode = MD_AD9833::MODE_TRIANGLE;  // triangle wave
  AD.setMode(mode);         // set the wave form
  AD.setFrequency(chan, freq);      // set signal frequency for channel
  delay(timeint);           // time to generate signal
  clear();              // call clear function
}

void clear()            // function to clear signal
{
  mode = MD_AD9833::MODE_OFF;       // set wave mode to off
  AD.setMode(mode);         // turn off wave generation
  delay(500);           // time with no signal
}
