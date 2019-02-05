// event dispatcher.

#define    EVENT _00,_01,_02,_03,_04,_05,_06,_07,_08,_09,_10,_11

#define   _BOOT        0       // _00() Initialize the adjustment mode
#define   _IDLE        1       // _01() idling
#define   _SELECT      2       // _02() select the commands
#define   _DISPATCH    3       // _03() carry out the selected commands
#define   _YEAR        4       // _04() adjust year
#define   _MONTH       5       // _05() adjust month
#define   _DATE        6       // _06() adjust date
#define   _HOUR        7       // _07() adjust hour
#define   _MINUTE      8       // _08() adjust minute
#define   _SECOND      9       // _09() adjust second
#define   _AMPMMT     10       // _10() change AM/PM MT mode
#define   _DUMMY      11       // _11() prevent long press

static uint8_t EventID;
void ( *event[] )() = { EVENT };
void event_dispatcher( void ) { (*event[ EventID ])(); }

static uint8_t TemporaryEventID;
void event_switcher( uint8_t e ) {
  TemporaryEventID = e;
  EventID = _DUMMY;
}