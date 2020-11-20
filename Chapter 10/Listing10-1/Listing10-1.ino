/*******************************************************************************
 * Sketch name: Servo motors and Bluetooth
 * Description: app control of servo motors
 * Created on:  October 2020
 * Author:      Neil Cameron
 * Book:        Electronics Projects with the ESP8266 and ESP32
 * Chapter :    10 - Build an app
 ******************************************************************************/

#include <Servo.h>          // include Servo library
Servo servoFB;            // associate servos with library
Servo servoLR;
int FBpin = D3;           // servo motor pins
int LRpin = D4;
int FBpos[20];            // arrays for saved servo positions
int LRpos[20];
int Nservo = 0;           // number of saved positions
int FB, LR, steps, stepsize;
char c;

void setup()
{
  Serial.begin(9600);         // Bluetooth module baud rate
  servoFB.attach(FBpin);        // servo motor pin to Servo lib
  servoLR.attach(LRpin);
  startPosition();          // set initial servo positions
}

void loop()
{
  if(Serial.available()>0) c = Serial.read(); // read character in Serial buffer
       if(c == 'U') FB = FB-5;      // move servo forward (Up)
  else if(c == 'D') FB = FB+5;      // move servo backward (Down)
  else if(c == 'L') LR = LR+5;      // move servo left
  else if(c == 'R') LR = LR-5;      // move servo right
  else if(c == 'Z') delay(100);     // stop moving both servos
  else if(c == 'S') saveServo();      // save both servo positions
  else if(c == 'P') playServo(15);      // playback servo positions
  else if(c == 'E') resetServo();     // reset saved positions
  if(c != 'Z' && c !=' ') moveServo(FB, LR, 100); // move both servos
}

void startPosition()        // function to set initial servo positions
{
  FB = 50;              // arbitrary home position
  LR = 70;
  moveServo(FB, LR, 100);       // move servos to initial position
}

void moveServo(int vFB, int vLR, int lag)   // function to move servos
{
  vFB = constrain(vFB, 5, 100);     // constrain servo positions
  vLR = constrain(vLR, 5, 175);
  servoFB.write(vFB);         // move forward-backward servo
  delay(lag);           // time between servo movements
  servoLR.write(vLR);         // move left-right servo
  delay(lag);
}

void saveServo()            // function to save servo positions
{
  Nservo++;             // increment number of positions
  Serial.println(Nservo);       // transmit position number to app
  FBpos[Nservo] = FB;         // save forward-backward posit.
  LRpos[Nservo] = LR;         // save left-right position
  c = ' ';              // reset command value
}

void playServo(int lag)       // function to play back servo positions
{
  startPosition();          // move servos to initial position
  FBpos[0] = FB;
  LRpos[0] = LR;
  for (int i=1; i<Nservo+1; i++)      // cycle through saved positions
  {
    if(FBpos[i] != FBpos[i-1])      // forward-back position change
    {
      steps = abs((FBpos[i] - FBpos[i-1])/5);   // number of steps
      stepsize = 5;
      if(FBpos[i] < FBpos[i-1]) stepsize = -5;    // magnitude of step size
// change in FB from FBpos[i-1],LRpos[i-1]
      for (int j = 0; j<steps; j++)
moveServo(FBpos[i-1]+j*stepsize, LRpos[i-1], lag);
    }
    if(LRpos[i] != LRpos[i-1])      // left-right position change
    {
      steps = abs((LRpos[i] - LRpos[i-1])/5);
      stepsize = 5;
      if(LRpos[i] < LRpos[i-1]) stepsize = -5;
// now at FBpos[i], so change in LR to LRpos[i]
      for (int j = 0; j<steps; j++)
moveServo(FBpos[i], LRpos[i-1]+j*stepsize, lag);
    }
  }
  c = ' ';              // reset command value
}

void resetServo()         // function to reset saved positions
{
  Nservo = 0;           // reset position number to zero
  Serial.println(Nservo);       // transmit position number to app
  c = ' ';              // reset command value
}
