/* name      : clock_4d_led_03_1.ino
   purpose   : Clock with 4 digit LED display (TM1637), using Arduino and RTC DS1307
   author    : LZ2WSG Veselin Georgiev
   site      : http://kn34pc.com/construct/lz2wsg_arduino_clock_4d_led.html
   date      : 2-12-2018
   revision  : v1.03.1
   used info : http://www.manelsoft.com/projects/arduino_ds1307_clock.aspx
             : http://playground.arduino.cc/Main/TM1637
             : http://www.ad7c.com/projects/ad9850-dds-vfo/
  ---------------------------------------------------------------------------------------------------------*/
#include <Wire.h>
#include "RTClib.h"                              // https://github.com/adafruit/RTClib
#include "TM1637.h"                              // https://github.com/avishorp/TM1637

#define CLK1 9                                    // TM1637 pin CLK1 ECRAN HEURE
#define DIO1 8                                    // TM1637 pin DIO1 ECRAN HEURE

#define CLK2 6                                    // TM1637 pin CLK2 ECRAN DATE
#define DIO2 7                                    // TM1637 pin DIO2 ECRAN DATE  

#define CLK3 10                                   // TM1637 pin CLK3 ECRAN ANNEE
#define DIO3 11                                    // TM1637 pin DIO3 ECRAN ANNEE

#define SW_HOUR A0                               // Set HOURS button
#define SW_MIN  A1                               // Set MINUTES button

#define SW_MONTH A2                               // Set DAY button
#define SW_DAY  A3                               // Set Month button

#define SW_YEAR A4                               // Set YEAR button

TM1637 tm1637(CLK1, DIO1);
TM1637 tm1637b(CLK2, DIO2);
TM1637 tm1637c(CLK3, DIO3);

RTC_DS1307 RTC;                                  // RTC DS1307 use SDA(A4) and SCL(A5) pins

bool dp = true;                                  // digital points
bool rep = false;
uint8_t m;                                       // minutes
uint8_t h;                                       // hours
uint8_t mo;                                      // month
uint8_t d;                                       // day
uint8_t y;                                       // year
uint8_t tmp;
uint32_t vr = 0;                                 // time variable
//---------------------------------------------------------------------------------------------------------
void setup()
{
  pinMode(SW_HOUR, INPUT_PULLUP);
  pinMode(SW_MIN, INPUT_PULLUP);
  pinMode(SW_MONTH, INPUT_PULLUP);
  pinMode(SW_DAY, INPUT_PULLUP);
  pinMode(SW_YEAR, INPUT_PULLUP);

  Wire.begin();                                  // Initiate the Wire library and join the I2C bus

  RTC.begin();                                   // Initiate the RTC library
  // RTC.adjust(DateTime(__DATE__, __TIME__));

  tm1637.init();                                 // Initiate the TM1637 library
  tm1637.set(BRIGHTEST);                         // BRIGHT_TYPICAL = 2, BRIGHT_DARKEST = 0, BRIGHTEST = 7
  tm1637b.init();                                 // Initiate the TM1637 library
  tm1637b.set(BRIGHTEST);                         // BRIGHT_TYPICAL = 2, BRIGHT_DARKEST = 0, BRIGHTEST = 7
  tm1637c.init();                                 // Initiate the TM1637 library
  tm1637c.set(BRIGHTEST);                         // BRIGHT_TYPICAL = 2, BRIGHT_DARKEST = 0, BRIGHTEST = 7
}
//---------------------------------------------------------------------------------------------------------
void loop()
{
  if (millis() - vr > 500) {                     // time refresh (and digital points blink): every 0,5 sec
    show_time();
    tm1637.point(dp);
    dp = !dp;
    vr = millis();
  }
  show_day();
  show_month();
  show_year();
  
  if (read_button(SW_HOUR) > 0) {
    DateTime now = RTC.now();
    tmp = now.hour();
    if (tmp == 23)
      tmp = 0;
    else
      tmp++;
    RTC.adjust(DateTime(now.year(), now.year(), now.day(), tmp, now.minute(), now.second()));
    show_time();
    delay(10);
  }

  if (read_button(SW_MIN) > 0) {
    DateTime now = RTC.now();
    tmp = now.minute();
    if (tmp == 59)
      tmp = 0;
    else
      tmp++;
    RTC.adjust(DateTime(now.year(), now.year(), now.day(), now.hour(), tmp, 0));
    show_time();
    delay(10);
  }
 if (read_button(SW_DAY) > 0) {
    DateTime now = RTC.now();
    tmp = now.day();
    if (tmp ==  31)
      tmp = 0;
    else
      tmp++;
    RTC.adjust(DateTime(now.year(), now.month(), tmp, now.hour(), now.minute()));  
    show_day();
    delay(10);
 }   
  if (read_button(SW_MONTH) > 0) {
    DateTime now = RTC.now();
    tmp = now.month();
    if (tmp ==  12)
      tmp = 1;
    else
      tmp++;
    RTC.adjust(DateTime(now.year(), tmp, now.day(), now.hour(), now.minute()));  
    show_month();
    delay(10);  
  }




 
  if (rep) {
    if (digitalRead(SW_HOUR) && digitalRead(SW_MIN) && digitalRead(SW_DAY)&& digitalRead(SW_MONTH)&& digitalRead(SW_YEAR))
      rep = false;
  }
}
//---------------------------------------------------------------------------------------------------------
void show_time()                                 // Time Format -> HH : MM
{
  DateTime now = RTC.now();

  h = now.hour();
  m = now.minute();  
  tm1637.display(0, h / 10);
  tm1637.display(1, h % 10);
  tm1637.display(2, m / 10);
  tm1637.display(3, m % 10);
}
void show_day()                                 // Time Format -> HH : MM
{
  DateTime now = RTC.now();
  h = now.hour();
  m = now.minute();
  d = now.day();
  mo = now.month();
  tm1637b.display(0, d / 10);
  tm1637b.display(1, d % 10);
  tm1637b.display(2, mo / 10);
  tm1637b.display(3, mo % 10);
}
void show_month()                                 // Time Format -> HH : MM
{
  DateTime now = RTC.now();
  h = now.hour();
  m = now.minute();
  d = now.day();
  mo = now.month();
  tm1637b.display(0, d / 10);
  tm1637b.display(1, d % 10);
  tm1637b.display(2, mo / 10);
  tm1637b.display(3, mo % 10);
}
void show_year()                                 // Time Format -> HH : MM
{
  DateTime now = RTC.now();
  h = now.hour();
  m = now.minute();
  d = now.day();
  mo = now.month();
  y = now.year();
  tm1637c.display(0, y / 10);
  tm1637c.display(1, y % 10);
  tm1637c.display(2, y / 10);
  tm1637c.display(3, y % 10);
}



//---------------------------------------------------------------------------------------------------------
int read_button (uint8_t pin_wbutton) {     // push button state, return: 0(very fast), 1(fast), 2(slow)
  uint32_t vr_start = 0;
  uint32_t vr_button = 0;
  uint8_t  wposition = 0;
  uint16_t wzab;

  if (rep)
    wzab = 200;
  else
    wzab = 1000;

  vr_start = millis();
  while (digitalRead(pin_wbutton) == LOW) {
    vr_button = millis() - vr_start;
    if (vr_button > wzab) {
      rep = true;
      break;
    }
  }
  if (vr_button < 30)                       // 'nothing' for debounce
    wposition = 0;
  else if (vr_button > 100)                 // slow push button
    wposition = 2;
  else wposition = 1;                       // fast push button

  return wposition;
}
