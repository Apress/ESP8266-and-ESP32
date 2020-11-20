/*******************************************************************************
 * Sketch name: XML HTTP requests for BMP280 temperature, counter and LED state
 * Description: Revised functions for Listing 8-1
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    8 - Updating a webpage
 ******************************************************************************/

void base()            // function to load default webpage
{             // and send HTML code to client
  server.send (200, "text/html", page);
}

void tempFunct()          // function to get temperature reading
{             // and send value to client
  temp = String(bmp.readTemperature());
  server.send (200, "text/plain", temp);  // send plain text not HTML code
}

void countFunct()         // function to increment counter
{             // and send value to client
  counter = String(count++);
  server.send (200, "text/plain", counter);
}

void LEDfunct()         // function to update LED
{             // and send LED state to client
  digitalWrite(LEDpin, !digitalRead(LEDpin));
  if(LED == "on") LED = "off";
  else LED = "on";
  server.send (200, "text/plain", LED);
}
