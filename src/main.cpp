//www.qadroid.com 
//2023.5.11



/**
 * This is an alarm clock designed to ring after 8.5 hours of starting.
 * It will play a buzzer at the end of it's timer.
 * A button will be used to turn it off.
 * Planned to add lighting to the sound of buzzer.
 * A screen will show temperature and time remaining to alarm.  
 */

/**
 * A lot of this code is borrowed and modified from examples and other projects.
*/

// ----------------------------
// Imports and definitions
#include "Arduino.h"
#include "RTClib.h"
#include "Wire.h"
#include "LiquidCrystal.h"
#include "DHT_U.h"
#include "SPI.h"
#include "Tone.h"
#include "IRremote.hpp"
#include "AlarmTimer.h"
#include "TempSensor.h"
#include "LEDRow.h"
#include "DHTReader.h"
#define buzzer 6
// ----------------------------

// ----------------------------
// Defining units of time as well as timer length
int second = 1000;
int minute = 60 * second;
int hour = 60 * minute;
int alarmTimeRemaining = 8.5 * hour;
// ----------------------------

// ----------------------------
// Setting lock values to be switched later
int alarmActive = 0;
int alarmRinging = 0;
int alarmStopped = 1;
// ----------------------------

// ----------------------------
// Alarm buttons
int stopButton = 7;
int resetButton = 2;
// ----------------------------

// ----------------------------
// DHT sensor setup
int DHT_SENSOR_PIN = 1;
// ----------------------------

// ----------------------------
// IR sensor setup
int IR_SENSOR_PIN = 0;
// ----------------------------

// ----------------------------
// Liquid Crystal setup
//                BS E  D4  D5  D6  D7
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
// ----------------------------

// ----------------------------
// Buzzer setup
int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
int duration = 500;  // 500 miliseconds
// ----------------------------

// ----------------------------
// LED row setup (Not yet implemented)
int tDelay = 100;
int latchPin = 4;      // (4) ST_CP [RCK] on 74HC595
int clockPin = 3;      // (3) SH_CP [SCK] on 74HC595
int dataPin = 5;     // (5) DS [S1] on 74HC595

byte leds = 0;

void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
}
// ----------------------------

// The purpose of this function is to type out the early text and call the next functions for temp and timing.
// ----------------------------
void displayTimer()
{
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.setCursor(5, 0);
  // Implement temp sensor here later
  // Enter code here:
  
  lcd.setCursor(0, 1);
  lcd.print("Time left:");
  lcd.setCursor(10, 1);

  // Calculating time remaining
  char timeRemaining[] = "08:30";

  // Calculating hours remaining
  char hoursRemaining = alarmTimeRemaining / 60 * minute;

  // Applying hours to result
  if (alarmTimeRemaining >= 60 * minute) 
  {
    timeRemaining[1] = hoursRemaining;
  } else {
    timeRemaining[1] = '0';
  }

  // Calculating minutes remaining
  int minutesRemaining = alarmTimeRemaining / 60 * second;

  // Applying minutes to result
  if (alarmTimeRemaining >= 10 * minute) 
  {
    timeRemaining[3,4] = minutesRemaining;
  } else { 
    timeRemaining[4] = minutesRemaining;
  }

  // Print the result
  lcd.print(timeRemaining);
}
// ----------------------------

// ----------------------------
void setup()
{

  // This part is for the LED row
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);

  // This part is for the buttons
  pinMode(stopButton, INPUT_PULLUP);
  pinMode(resetButton, INPUT_PULLUP);

  // From the clock project
  Serial.begin(9600);
  Wire.begin();

  // Initialize lcd 
  lcd.begin(16, 2);
}
// ----------------------------

// ----------------------------
void loop() 
{
  displayTemp();
  displayTempTimer();
  delay(1000);
  if (digitalRead(resetButton) == LOW) 
  {
    if (alarmActive = 0)
    {
      alarmActive = 1;
      alarmStopped = 0;
    } else {
      alarmActive = 0;
      alarmTimeRemaining = 8.5 * hour;
    }
  }

  while (alarmActive == 1 && alarmTimeRemaining > 0) 
  {
    alarmTimeRemaining = alarmTimeRemaining - 1000;
    displayTempTimer();
    delay(1000);
    if (alarmTimeRemaining == 0) 
    {
      alarmRinging = 1;
      alarmActive = 0;
      while (alarmStopped == 0) 
      {
        for (int thisNote = 0; thisNote < 8; thisNote++) 
        {
          tone(buzzer, melody[thisNote], duration);
          delay(duration);
          noTone(buzzer);
        }
        if (digitalRead(stopButton) == LOW) 
        {
          alarmStopped = 1;
          alarmActive = 0;
          delay(1000);
        }
      }
    }
  }
  
}
// ----------------------------

