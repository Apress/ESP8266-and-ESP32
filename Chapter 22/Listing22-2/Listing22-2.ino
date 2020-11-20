/*******************************************************************************
 * Sketch name: Simultaneous determination of the first 5k and 10k prime numbers
 * Description: Allocating tasks to each ESP32 microcontroller core
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    22 - ESP32 microcontroller features
 ******************************************************************************/

unsigned long num5k = 2, num10k = 2;    // start from number 2
int count5k = 1, count10k = 1;      // prime number counters
unsigned int start5k = 0, start10k = 0;   // processing times

void setup()
{
  Serial.begin(115200);         // define Serial Monitor baud rate
  xTaskCreatePinnedToCore(code5k, "5k", 1000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(code10k, "10k", 1000, NULL, 1, NULL, 1);
}

void code5k(void * parameter)       // function for 5k primes
{
  for (;;)
  {
    num5k++;            // increment number to check
    int chk = is_prime(num5k);      // call function to test for prime
    if (chk > 0) count5k++;       // increment counter when prime
    if (count5k > 4999)         // count up to 5k numbers
    {
      printLine(start5k, count5k, num5k);   // function to display results
      num5k = 2;
      count5k = 1;          // reset parameters
      start5k = millis();
      vTaskDelay(1);          // delay for watchdog timer
    }
  }
}

void code10k(void * parameter)      // function for 10k primes
{
  for (;;)
  {
    num10k++; 
    int chk = is_prime(num10k);
    if (chk > 0) count10k++;
    if (count10k > 9999)
    {
      printLine(start10k, count10k, num10k);
      num10k = 2;
      count10k = 1;
      start10k = millis();
      vTaskDelay(1);
    }
  }
}

void printLine(unsigned long start, int count, unsigned long number)
{
  int ms = millis() - start;
  Serial.print("Core ");Serial.print(xPortGetCoreID());
  Serial.print(" Found ");Serial.print(count);
  Serial.print(" primes in "); Serial.print(ms);
  Serial.print(" ms");
  Serial.print(" highest prime is ");Serial.println(number);

}

int is_prime(unsigned long num)   // function to check if prime number
{
  int mod = num % 2;        // exclude even numbers
  if (mod == 0) return 0;
  int limit = sqrt(num);      // check divisors less than square root
  for (int divid = 3; divid <= limit; divid = divid + 2)
  {
    mod = num % divid;        // remainder after dividing
    if (mod == 0) return 0;     // not prime if zero remainder
  }
  return 1;           // no divisor with zero remainder
}

void loop()
{
  vTaskDelay(NULL);
}
