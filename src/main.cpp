//www.qadroid.com 
//2023.5.11



/**
 * This alarm clock will ring based on the time
 */


#include "Arduino.h"
#include <RTClib.h>


// Set LED_BUILTIN if it is not defined by Arduino framework
// #define LED_BUILTIN 13

// My program parts


// Clock from Arduino project
int hourupg;
int minupg;
int yearupg;
int monthupg;
int dayupg;
int menu =0;
int setAll =0;

uint8_t alarmHours = 0, alarmMinutes = 0;  // Holds the current alarm time

// This stuff is from Eight LEDs
int tDelay = 100;
int latchPin = 4;      // (4) ST_CP [RCK] on 74HC595
int clockPin = 3;      // (3) SH_CP [SCK] on 74HC595
int dataPin = 5;     // (5) DS [S1] on 74HC595

// Also Eight LEDs
byte leds = 0;

// Also Eight LEDs
void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
}

void setup()
{

  // This part is for Eight LEDs
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
}

void loop()
{
  // Eight LEDs
  leds = 0;
  updateShiftRegister();
  delay(tDelay);
  for (int i = 0; i < 6; i++)
  {
    bitSet(leds, i);
    updateShiftRegister();
    delay(tDelay);
  }
}
