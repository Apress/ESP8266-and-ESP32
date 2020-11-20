/*******************************************************************************
 * Sketch name: Determine first 10k prime numbers
 * Description: Sketch used as baseline to measure performance
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    21 - Microcontrollers
 ******************************************************************************/

int Nprimes = 9999;          // required number of primes - 1
unsigned long number = 2;       // start from number 2
int count = 1;            // prime number counter
unsigned int start = 0;         // store processing time
unsigned long ms;
int chk, limit, mod, divid;

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  while(!Serial);           // wait for Pro Micro to connect
  Serial.print("\nCPU "); Serial.println(F_CPU/1000000);
  start = millis();         // start of processing time
}

void loop()
{
  number++;             // increment number to check
  chk = is_prime(number);
  if (chk > 0) count++;         // increment counter when prime
  if (count > Nprimes)
  {
    ms = millis() - start;        // display results
    Serial.print("Found ");
    Serial.print(count);
    Serial.print(" primes in ");
    Serial.print(ms);
    Serial.println(" ms");
    Serial.print("Highest prime is ");
    Serial.println(number);
    delay(50000);           // long delay when finished
  }
}

int is_prime(unsigned long num)   // function to check if prime number
{
  mod = num % 2;          // exclude even numbers
  if (mod == 0) return 0;
  limit = sqrt(num);        // check divisors less than square root
  for (int divid = 3; divid <= limit; divid = divid + 2)
  {
    mod = num % divid;        // remainder after dividing
    if (mod == 0) return 0;     // not prime if zero remainder
  }
  return 1;           // no divisor with zero remainder
}
