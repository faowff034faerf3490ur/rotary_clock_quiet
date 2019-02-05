// rotary encoder

volatile uint8_t REGS1,REGS2;
volatile boolean CLK_PIN, DT_PIN;
volatile boolean Rotation = OFF;
volatile int16_t RotaryCounter;
volatile unsigned long ReleaseCounter;

void isr_0( void ) {
  REGS1  = PIND & 0b00001100;
  if ( ( REGS1 == 0b00001100 ) && CLK_PIN ) {
    RotaryCounter--; ReleaseCounter = 0;
    Rotation = ON; CLK_PIN = DT_PIN = LOW; sei();
  } else if ( REGS1 == _BV(2) ) DT_PIN = HIGH;
}

void isr_1( void ) {
  REGS2  = PIND & 0b00001100;
  if ( ( REGS2 == 0b00001100 ) && DT_PIN ) {
    RotaryCounter++; ReleaseCounter = 0;
    Rotation = ON; CLK_PIN = DT_PIN = LOW; sei();
  } else if ( REGS2 == _BV(3) ) CLK_PIN = HIGH;
}

#define RTMO 40  // Release time Maximum others.
#define RTMS 90  // Release time Maximum second.

static uint8_t ReleaseCounterMax = RTMO;

bool is_rotation( void ) {
  bool r = Rotation;
  if ( Rotation ) ReleaseCounter = ReleaseCounterMax;
  Rotation = OFF;
  return( r );
}

// builtin switch

static boolean DirectSwitch;
static boolean EdgeTriggerSwitch;
static boolean PreviousSwitch;

void check_switch_condition( void ) {
  DirectSwitch = !digitalRead( SW ); // convert to positive logic 
  EdgeTriggerSwitch = DirectSwitch ^ PreviousSwitch & DirectSwitch;   // rising
  PreviousSwitch = DirectSwitch;
}
