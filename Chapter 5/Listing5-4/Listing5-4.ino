/*******************************************************************************
 * Sketch name: Infrared remote control of MP3 player
 * Description: Revised loop function from Listing 5-1
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    5 - MP3 player
 ******************************************************************************/

void loop()
{
  if(finish == 1)            // current audio file ended
  {
    cmd(0x01, 0);           // play next track
    finish = 0;           // set finish variable
    cmd(0x4C, 0);           // get track number
  }
  if(irrecv.decode(&reading))       // read pulsed signal
  {
    if(reading.value == 0x8D1)      // next audio file is selected
    {
      file = file+1;          // increment file name
      if(file > fileMax) file = fileMin;    // constrain file name < fileMax
      cmd(0x12, file);          // play next audio file
      finish = 0;           // set finish variable
      cmd(0x4C, 0);         // get track number
    }
    else if(reading.value == 0x491)     // increase volume is selected
    {
      cmd(0x04, 0);         // increase volume
      cmd(0x43, 0);         // get volume value
    }
    else if(reading.value == 0xC91)     // decrease volume is selected
    {
      cmd(0x05, 0);         // decrease volume
      cmd(0x43, 0);         // get volume value
    }
    else if(reading.value == 0x1D1)     // change equaliser is selected
    {
      EQstate = EQstate+1;        // increment equaliser
      if(EQstate > 5) EQstate = 0;      // constrain equaliser value
      Serial.println(EQ[EQstate]);
      cmd(0x07, EQstate);       // change equaliser setting
    }
    else
    {
      switch(reading.value)       // switch case for selected track
      {
        case 0x011: track = 1; break;
        case 0x811: track = 2; break;
        case 0x411: track = 3; break;   // map remote signal to play track
        case 0xC11: track = 4; break;
        case 0x211: track = 5; break;
      }
      cmd(0x03, track);         // play track
      finish = 0;           // set finish variable
      cmd(0x4C, 0);         // get track number
    }
//    irrecv.resume();          // for ESP32, receive next value
  }
  delay(100);
}
