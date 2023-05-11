//www.qadroid.com 
//2023.5.11



/**
 * This alarm clock will ring based on the time.
 * It will play a buzzer at said times.
 * A button will be used to turn it off.
 * Lights will play along with the buzzer.
 * A screen will show temperature and 'weather info?' but will change when the alarm is active 
 */

/**
 * A lot of this code is borrowed from example code from Elegoo's and Arduino's resources.
*/


#include "Arduino.h"
#include <RTClib.h>
#include <LiquidCrystal.h>
#include "DHT_Async.h"
#include <SPI.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11


// Set LED_BUILTIN if it is not defined by Arduino framework
// #define LED_BUILTIN 13

// DHT setup
static const int DHT_SENSOR_PIN = 0;
DHT_Async dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );


// Alarm data
int alarmActive = 0;
int alarmStopped = 0;


// Liquid Crystal components
//                BS E  D4  D5  D6  D7
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

// RTC components
RTC_DS1307 RTC;

int hourupg;
int minupg;
int yearupg;
int monthupg;
int dayupg;
int menu =0;
int setAll =0;

uint8_t alarmHours = 0, alarmMinutes = 0;  // Holds the current alarm time

// LED row setup
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

void setup()
{

  // This part is for the LED row
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);

  // From the clock project
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // Set the date and time at compile time
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  // RTC.adjust(DateTime(__DATE__, __TIME__)); //removing "//" to adjust the time
    // The default display shows the date and time
  int menu=0;

  
}

/*
 * Poll for a measurement, keeping the state machine alive.  Returns
 * true if a measurement is available.
 */
static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
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

  // DHT void loop()
  float temperature;
  float humidity;

  /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
  if( measure_environment( &temperature, &humidity ) == true )
  {
    Serial.print( "T = " );
    Serial.print( temperature, 1 );
    Serial.print( " deg. C, H = " );
    Serial.print( humidity, 1 );
    Serial.println( "%" );
  }
if (alarmActive == 0)
{
  lcd.setCursor(0, 0);
  lcd.print("Temp         C  ");
  // Display Temperature in F
  //lcd.print("Temp         F  ");
  /* lcd.setCursor(6, 0);
  // Display Temperature in C
  lcd.print(temperature);
  // Display Temperature in F
  //lcd.print(tempF);
  delay(500); */
}
 

}
