// time settings.

/////////////////////////////////////////////////////////////////////////////////
//        BOOT          BOOT          BOOT          BOOT          BOOT
/////////////////////////////////////////////////////////////////////////////////
void _00( void ) // _BOOT
{
  StopFlag = OFF;
  ReleaseCounterMax = RTMO;  
  LongPushCounter = 0;
  ReleaseCounter = 0;
  select_inactive();
  clock_refresh();
  EventID++;
}
/////////////////////////////////////////////////////////////////////////////////
//        IDLE          IDLE          IDLE          IDLE          IDLE
/////////////////////////////////////////////////////////////////////////////////
void _01( void )  // _IDLE
{
  if ( !EdgeTriggerSwitch ) return;
  RotaryCounter = _YEAR;
  select_active();
  EventID++;
}
/////////////////////////////////////////////////////////////////////////////////
//        SELECT        SELECT        SELECT        SELECT        SELECT
/////////////////////////////////////////////////////////////////////////////////
void _02( void )  // _SELECT
{
  if ( is_rotation() ) return;
  if ( RotaryCounter < _YEAR ) RotaryCounter = _SECOND;
  if ( RotaryCounter > _SECOND ) RotaryCounter = _YEAR;
  switch ( RotaryCounter ) {
    case _YEAR     : lcd.setCursor( 3, 0 ); break;
    case _MONTH    : lcd.setCursor( 7, 0 ); break;
    case _DATE     : lcd.setCursor( 10,0 ); break;
    case _HOUR     : lcd.setCursor( 4, 1 ); break;
    case _MINUTE   : lcd.setCursor( 7, 1 ); break;
    case _SECOND   : lcd.setCursor( 10,1 ); break;
  }
  if ( EdgeTriggerSwitch ) EventID++;
}
/////////////////////////////////////////////////////////////////////////////////
//        DISPATCH        DISPATCH        DISPATCH        DISPATCH
/////////////////////////////////////////////////////////////////////////////////
void _03( void ) // _DISPATCH
{
  EventID = RotaryCounter;
  switch ( EventID )
  {
    case _YEAR    : RotaryCounter = clock.getYear() + 2000 ; break;
    case _MONTH   : RotaryCounter = clock.getMonth( Century ); break;
    case _DATE    : RotaryCounter = clock.getDate(); break;
    case _HOUR    : RotaryCounter = clock.getHour( h12, PM ); break;
    case _MINUTE  : RotaryCounter = clock.getMinute(); break;
    case _SECOND  : RotaryCounter = clock.getSecond(); break;
    default       : break;
  }
  lcd.cursor();
  lcd.noBlink();
}
/////////////////////////////////////////////////////////////////////////////////
//        YEAR        YEAR        YEAR        YEAR        YEAR        YEAR
/////////////////////////////////////////////////////////////////////////////////
void _04( void )  // _YEAR
{
  if ( RotaryCounter < 2000 ) RotaryCounter = 2099;
  if ( RotaryCounter > 2099 ) RotaryCounter = 2000;
  if ( is_rotation() ) {
    clock.setYear( RotaryCounter - 2000 );
    lcd.setCursor( 0, 0 );
    lcd.print( RotaryCounter );
    print_dow_str();
  }
  lcd.setCursor( 3, 0 );
  if ( !EdgeTriggerSwitch ) return;
  RotaryCounter = clock.getMonth( Century );
  event_switcher( _MONTH );
}
/////////////////////////////////////////////////////////////////////////////////
//        MONTH        MONTH        MONTH        MONTH        MONTH
/////////////////////////////////////////////////////////////////////////////////
void _05( void )  // _MONTH
{
  if ( RotaryCounter > 12 ) RotaryCounter =  1;
  if ( RotaryCounter < 1  ) RotaryCounter = 12;
  if ( is_rotation() ) {
    clock.setMonth( RotaryCounter );
    print_mon_str();
    print_dow_str();
  }
  lcd.setCursor( 7, 0 );
  if ( !EdgeTriggerSwitch ) return;
  RotaryCounter = clock.getDate();
  event_switcher( _DATE );
}
/////////////////////////////////////////////////////////////////////////////////
//        DATE        DATE        DATE        DATE        DATE        DATE
/////////////////////////////////////////////////////////////////////////////////
void _06() // _DATE
{
  static uint8_t dom[] = { 00, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  if ( ( clock.getYear() % 4 ) == 0 ) dom[2] = 29; else dom[2] = 28;
  if ( RotaryCounter > dom[ clock.getMonth(Century) ] ) RotaryCounter = 1;
  if ( RotaryCounter <  1 ) RotaryCounter = dom[ clock.getMonth(Century) ];
  if ( is_rotation() ) {
    print_date();
    clock.setDate( RotaryCounter );
    print_dow_str();
  }
  lcd.setCursor( 10, 0 );
  if ( !EdgeTriggerSwitch ) return;
  RotaryCounter = clock.getHour( h12, PM );
  event_switcher( _HOUR );
}
/////////////////////////////////////////////////////////////////////////////////
//        HOUR        HOUR        HOUR        HOUR        HOUR        HOUR
/////////////////////////////////////////////////////////////////////////////////
void _07( void ) // _HOUR
{
  if ( RotaryCounter > 23 ) RotaryCounter =  0;
  if ( RotaryCounter <  0 ) RotaryCounter =  23;
  if ( is_rotation() ) {
    print_hour();
    clock.setHour( RotaryCounter );
    print_apm_str();
  }
  lcd.setCursor( 4, 1 );
  if ( !EdgeTriggerSwitch ) return;
  RotaryCounter = clock.getMinute();
  event_switcher( _MINUTE );
}
/////////////////////////////////////////////////////////////////////////////////
//        MINUTE        MINUTE        MINUTE        MINUTE        MINUTE
/////////////////////////////////////////////////////////////////////////////////
void _08( void ) // _MINUTE
{
  if ( RotaryCounter > 59 ) RotaryCounter =  0;
  if ( RotaryCounter <  0 ) RotaryCounter = 59;
  if ( is_rotation() ) {
    clock.setMinute( RotaryCounter );
    print_min();
  }
  lcd.setCursor( 7, 1 );
  if ( !EdgeTriggerSwitch ) return;
  RotaryCounter = clock.getSecond();
  event_switcher( _SECOND );
}
/////////////////////////////////////////////////////////////////////////////////
//        SECOND        SECOND        SECOND        SECOND        SECOND
/////////////////////////////////////////////////////////////////////////////////
void _09( void ) // _SECOND
{
  if ( Rotation ) StopFlag = ON;
  if ( RotaryCounter > 59 ) RotaryCounter =  0;
  if ( RotaryCounter <  0 ) RotaryCounter = 59;
  if ( is_rotation() ) {
    lcd.setCursor( 9, 1 );
    add_zero( RotaryCounter );
  }
  lcd.setCursor( 10, 1 );
  ReleaseCounterMax = RTMS;
  if ( !EdgeTriggerSwitch ) return;
  clock.setSecond( RotaryCounter );
  StopFlag = OFF;
  event_switcher( _BOOT );
}
/////////////////////////////////////////////////////////////////////////////////
//        AMPMMT        AMPMMT        AMPMMT        AMPMMT        AMPMMT
/////////////////////////////////////////////////////////////////////////////////
void _10( void )  // _AMPMMT
{
  Military = !Military;
  print_apm_str();
  print_hour();
  if ( EdgeTriggerSwitch ) return;
  event_switcher( _BOOT );
}
/////////////////////////////////////////////////////////////////////////////////
//        _DUMMY        _DUMMY        _DUMMY        _DUMMY        _DUMMY
/////////////////////////////////////////////////////////////////////////////////
void _11( void ) // _DUMMY
{
  if ( DirectSwitch ) { lcd.noCursor(); lcd.noBlink(); return; }
  EventID = TemporaryEventID;
  lcd.cursor();
}