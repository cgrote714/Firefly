#include <Servo.h>

Servo Servo1;


// PWM pins are 3,5,6,9,10,11; 3 is INT, 10 & 11 are SPI.
// Servo library disables analogWrite on pins 9 and 10.  5 or 6 seem best for LED.

const int pinLED = 11;
const int pinServo = 5;
const int aSpeed = 1;
const int aBright = 2;

const long minLED = 1; //minimum LED brightness - aBright range
const long maxLED = 255; //maximum LED brightness - aBright range

const long minOff = 200; //minimum LED off time in milliseconds
const long maxOff = 7000; //maximum LED off time in milliseconds
const long minOn = 300; //minimum LED on time in milliseconds
const long maxOn = 1500; //maximum LED on time in milliseconds
const long minFade = 0; //minimum LED fade time in milliseconds
const long maxFade = 1000; //maximum LED fade time in milliseconds

const int forward = 1;
const int forward_wait = 2;
const int reverse = -1;
const int reverse_wait = -2;

long LED_current;
long LED_start_time;
long LED_start;
long LED_end_time;
long LED_end;
int LED_direction;

const long maxUS = 700; //minimum servo angle in microseconds
const long minUS = 2400; //maximum servo angle in microseconds
const long minSpeed = 3000; //minimum servo speed in milliseconds - aSpeed range
const long maxSpeed = 200; //maximum servo speed in milliseconds - aSpeed range

long servo_current;
long servo_start_time;
long servo_start;
long servo_end_time;
long servo_end;
int servo_direction;

void setup()
{
  Serial.begin(9600);
  
  Servo1.attach(pinServo);
  pinMode(pinLED, OUTPUT);
  pinMode(aSpeed, INPUT);
  pinMode(aBright, INPUT);
  
  randomSeed(analogRead(0)); //seed random number generator from floating unused analog input
  
  servo_direction = reverse;
  servo_current = minUS;
  servo_start = servo_current;
  servo_end = maxUS;
  servo_end_time = millis();
  //Servo1.writeMicroseconds(minUS);
  
  analogWrite(pinLED,maxLED);
  delay(1000);
  analogWrite(pinLED,minLED);
  delay(2000);
  analogWrite(pinLED,0);
  delay(2000);
  
  LED_direction = reverse;
  LED_current = 0;
  LED_end_time = millis();
  
  Update();
    
}

void loop()
{
  Update();
}

void Update()
{
  
  if(millis() >= LED_end_time)
  {
    if(LED_direction == forward)
    {
      LED_direction = forward_wait;
      LED_end = LED_current;
      LED_end_time = millis() + random(minOn, maxOn);
    }
    else if(LED_direction == forward_wait)
    {
      LED_direction = reverse;
      LED_end = maxLED * -1;
      LED_end_time = millis() + random(minFade, maxFade);
    }
    else if(LED_direction == reverse)
    {
      LED_direction = reverse_wait;
      LED_end = LED_current;
      LED_end_time = millis() + random(minOff, maxOff);
    }
    else if(LED_direction == reverse_wait)
    {
      LED_direction = forward;
      //LED_end = random(minLED, map(analogRead(aBright),0,255,minLED,maxLED));
      LED_end = random(minLED, maxLED);
      LED_end_time = millis() + random(minFade, maxFade);  
    }
    
    LED_start = LED_current;
    LED_start_time = millis();
  }
  
   if(millis() >= servo_end_time)
  {
    if(servo_direction == forward)
    {
      servo_direction = reverse;
      servo_end = minUS;
    }
    else if(servo_direction == reverse)
    {
      servo_direction = forward;
      servo_end = maxUS;
    }
    
    servo_start = servo_current;
    
    servo_end_time = millis() + minSpeed; //map(analogRead(aSpeed),0,255,minSpeed,maxSpeed); 
    servo_start_time = millis();
  } 
  
  LED_current = map(millis(),LED_start_time, LED_end_time, LED_start, LED_end);
  if(LED_current < minLED){LED_current = 0;}
  
  servo_current = map(millis(),servo_start_time, servo_end_time, servo_start, servo_end);
  
  Servo1.writeMicroseconds(servo_current);
  analogWrite(pinLED,LED_current); 
  
  //Debug();
  
}

void Debug()
{
  Serial.print(millis());Serial.print(" ");
  
  //Serial.print(LED_current);Serial.print(" ");
  //Serial.print(LED_end_time);Serial.print(" ");
  
  Serial.print(servo_current);Serial.print(" ");
  Serial.print(servo_start_time);Serial.print(" ");
  Serial.print(servo_end_time);Serial.print(" ");
  Serial.print(servo_direction);Serial.print(" ");
  Serial.print(servo_start);Serial.print(" ");
  Serial.print(servo_end);Serial.print(" ");  
  
  Serial.println("");
  delay(100);
}

