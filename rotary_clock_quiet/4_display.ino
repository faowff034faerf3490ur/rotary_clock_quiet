// LCD display.

// several abbreviations.
#define months_of_the_year "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
#define days_of_the_week   "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"

// for the purpose of correction a leap year.
#define days_of_the_month  00, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 // ordinary year
//                         00, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 // leap year

// days, weeks and months of the year
static char   *moy[] = { months_of_the_year };
static char   *dow[] = { days_of_the_week   };
static uint8_t dom[] = { days_of_the_month  };

void display_clock ( void )
{
  static unsigned long pms, cms; cms = millis();
  if ( cms - pms < 50 ) return; else  pms = cms;
  static DateTime p;
  static DateTime t;
  t = rtc.now();
  print_sec();
  if ( t.minute() != p.minute() ) print_min();
  if ( t.hour() != p.hour() ) { print_hour(); print_apm_str(); }
  if ( t.day() != p.day() ) { print_date(); print_dow_str(); }
  if ( t.month() != p.month() ) print_mon_str();
  if ( t.year() != p.year() ) print_year();
  print_temperature();
  p = t;
}

bool Century = OFF;
bool h12;
bool PM;

void print_year( void ) {
  lcd.setCursor( 0, 0 );
  lcd.print( 2000 + clock.getYear(), DEC );
}
void print_mon_str( void ) {
  lcd.setCursor( 5, 0 );
  lcd.print( moy[clock.getMonth(Century) - 1 ] );
}
void print_date( void ) {
  lcd.setCursor( 9, 0 );
  add_zero( clock.getDate() );
}
void print_dow_str( void ) {
  lcd.setCursor( 12, 0 );
  set_dow();
  lcd.print( dow[ clock.getDoW( ) - 1 ] );
}

static boolean Military = OFF;

void print_apm_str( void ) {
  lcd.setCursor( 0, 1 );
  if ( Military == OFF ) {
    if ( clock.getHour(h12, PM) > 11 ) lcd.print( "PM ");
    else lcd.print( "AM " );
  } else lcd.print( "MT ");
}
void print_hour( void ) {
  uint8_t val = clock.getHour(h12, PM);
  if ( Military == OFF ) {
    if ( clock.getHour(h12, PM) > 12 ) val -= 12;
    if ( clock.getHour(h12, PM) == 0 ) val  = 12;
  }
  lcd.setCursor( 3, 1 );
  add_zero( val );
  lcd.print( ":" );
}
void print_min( void ) {
  lcd.setCursor( 6, 1 );
  add_zero( clock.getMinute() );
  lcd.print( ":" );
}

bool StopFlag;

void print_sec( void ) {
  if ( StopFlag ) return;
  lcd.setCursor( 9, 1 );
  add_zero( clock.getSecond() );
}

static int8_t PrevTemp;

void print_temperature( void )
{
  static int8_t currTemp;
  currTemp = ( int8_t )clock.getTemperature();
  if ( PrevTemp == currTemp ) return;
  PrevTemp = currTemp;
  lcd.setCursor( 11, 1 );
  if ( currTemp < 0 )  lcd.print("-");
  else lcd.print(" ");
  add_zero( abs(currTemp) );
  lcd.setCursor( 14, 1 );
  lcd.print( char(223) );
  lcd.print( "C" );
}
void clock_refresh( void )
{
  PrevTemp = 100;
  print_sec();
  print_min();
  print_hour();
  print_apm_str();
  print_dow_str();
  print_date();
  print_mon_str();
  print_year();
  print_temperature();
}
void set_dow( void )
{
  int dow;
  byte mArray[] = { 6, 2, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
  DateTime t = rtc.now();
  dow = ( t.year() % 100);
  dow = dow * 1.25;
  dow += t.day();
  dow += mArray[ t.month() - 1];
  if ((( t.year() % 4 ) == 0 ) && ( t.month() < 3 ) ) dow -= 1;
  while (dow > 7) dow -= 7;
  clock.setDoW( dow );
}
void add_zero( uint8_t val ) {
  if ( val < 10 ) lcd.print( "0" );
  lcd.print( val );
}
void select_active( void )
{
  lcd.blink();
  lcd.noCursor();
  lcd.setCursor( 15, 0 );
  lcd.print( "." );
}
void select_inactive( void )
{
  lcd.noCursor();
  lcd.noBlink();
  lcd.setCursor( 15, 0 );
  lcd.print( " " );
}
