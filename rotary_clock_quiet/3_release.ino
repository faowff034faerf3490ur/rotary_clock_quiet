// rotary encoder switching mechanism.

#define INTERVAL_MILLIS 100
#define LONG_PUSH_COUNTER_MAX 8
static uint16_t LongPushCounter;

void long_push_timer ( void )
{
  if ( !DirectSwitch ) { LongPushCounter = 0; return; }
  static unsigned long pms, cms; cms = millis();
  if ( cms - pms < INTERVAL_MILLIS ) return; else pms = cms;
  if ( LongPushCounter == LONG_PUSH_COUNTER_MAX - 1 ) EventID = _AMPMMT;
  if ( LongPushCounter < LONG_PUSH_COUNTER_MAX ) LongPushCounter++;
}

void release_timer ( void )
{
  if ( DirectSwitch || Rotation ) { ReleaseCounter = ReleaseCounterMax; return; }
  static unsigned long pms, cms; cms = millis();
  if ( cms - pms < INTERVAL_MILLIS ) return; else pms = cms;
  if ( ReleaseCounter  > 0 ) ReleaseCounter--;
  if ( ReleaseCounter == 1 ) {
    EventID = _BOOT;
  }
}
