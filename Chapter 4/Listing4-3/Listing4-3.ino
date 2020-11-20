/*******************************************************************************
 * Sketch name: 74HC4051 multiplexer and ESP8266 development board
 * Description: Several analog inputs to ESP8266 microcontroller
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    4 - Internet clock
 ******************************************************************************/

int Spin[] = {D0, D1, D2};        // multiplexer S0, S1 and S2
                // switch pins
void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  for (int i=0; i<3; i++)
  {
    pinMode(Spin[i], OUTPUT);       // multiplexer pins as OUTPUT
    digitalWrite(Spin[i], LOW);     // set multiplexer pins to LOW
  }
}

void loop()
{
  for (int i=0; i<8; i++)       // 8 multiplexer pin combinations
  {               // set pins from bit sequence
    for (int j=0; j<3; j++) digitalWrite(Spin[j], bitRead(i, j));
    delay(10);
    Serial.print(analogRead(A0));Serial.print("\t");    // display readings
    for (int j=0; j<3; j++) digitalWrite(Spin[j], LOW); // reset pins
  }
  Serial.println();
  delay(1000);            // delay between readings
}
