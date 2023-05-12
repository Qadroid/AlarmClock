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
 * A lot of this code could be borrowed and modified from examples and other projects.
*/

// ----------------------------
// Imports and definitions
#include "Arduino.h"
#include "RTClib.h"
#include "Wire.h"
#include "LiquidCrystal.h"
#include "SPI.h"
#include "Tone.h"
#include "IRremote.hpp"
#include "AlarmTimer.h"
#include "TempSensor.h"
#include "LEDRow.h"
#include "DHTReader.h"
#include "Adafruit_Sensor.h"
#include "DHT.h"
#include "DHT_U.h"
// ----------------------------

// ----------------------------
// DHT setup
#define DHTPIN 1
#define DHTTYPE DHT11

DHT_Unified dht(DHTPIN, DHTTYPE);
// ----------------------------

// ----------------------------
// Alarm setup

// Defining units of time as well as timer length
int second = 1000;
int minute = 60 * second;
int hour = 60 * minute;
int alarmTimeRemaining = 8.5 * hour;

// Alarm status 'switches'
int alarmActive = 0;
int alarmRinging = 0;
int alarmStopped = 1;

// Alarm button setup
int stopButton = 7;
int resetButton = 2;
// ----------------------------

// ----------------------------
// IR sensor setup (Not yet implemented)
int IR_SENSOR_PIN = 0;
// ----------------------------

// ----------------------------
// Liquid Crystal setup
//                BS E  D4  D5  D6  D7
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
// ----------------------------

// ----------------------------
// Buzzer setup
#define buzzer 6

// Melody setup
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



void setup()
{

  // This part is for the LED row (can be ignored)
/*   pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT); */

  // This part is for the buttons
  pinMode(stopButton, INPUT_PULLUP);
  pinMode(resetButton, INPUT_PULLUP);

  // From the clock project
  Serial.begin(9600);
  Wire.begin();

  // Initialize lcd 
  lcd.begin(16, 2);

  // Initialize DHT sensor
  Serial.begin(9600);
  // Initialize device.
  dht.begin();
  // Print temperature sensor details.
  
}



void loop() 
{
  displayTemp();
  displayTimer();
  delay(2500);
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
    alarmTimeRemaining = alarmTimeRemaining - 2500;
    displayTemp();
    displayTimer();
    delay(2500);
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
  Serial.println("Loop ended");
}


