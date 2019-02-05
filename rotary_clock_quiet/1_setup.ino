// 1602 Liquid Crystal Display ( https://github.com/johnrickman/LiquidCrystal_I2C )

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd( 0x27,16,2 );
void setup_lcd( void ) { lcd.init(); lcd.backlight(); }

// DS3231 Real Time Clock ( https://github.com/NorthernWidget/DS3231 )

#include <DS3231.h>
DS3231 clock; RTClib rtc;
void setup_rtc( void ) {  Wire.begin(); set_dow(); }

// KY-040 Rotary Encoder

#define CLK 2
#define DT  3
#define SW  4

void setup_encoder( void ) {
  pinMode( CLK, INPUT_PULLUP );
  pinMode( DT,  INPUT_PULLUP );
  pinMode( SW,  INPUT_PULLUP );
  attachInterrupt( 0, isr_0, RISING );
  attachInterrupt( 1, isr_1, RISING );
}