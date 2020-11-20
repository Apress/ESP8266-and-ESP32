/*******************************************************************************
 * Sketch name: Variable, pointer and memory address
 * Description: Example use of a pointer
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    14 - ESP-NOW and LoRa communication
 ******************************************************************************/

int sum = 25;            // allocate value to variable
int *pointer;           // define pointer
int number;

void setup()
{
  Serial.begin(115200);
  pointer = &sum;           // set pointer to address of sum
  number = *pointer;          // set number to pointer content
  Serial.print("\nnumber ");Serial.println(number);
}

void loop()             // nothing in loop function
{}
