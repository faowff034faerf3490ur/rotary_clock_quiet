//    Arduino simple rotary clock ( without passive buzzer version )
//
//    2019 Jan 10 Thu'
//    MT 07:55:36 18˚C
//
//    https://youtu.be/sK5B9zZ70d4

void setup() {
  setup_lcd();
  setup_rtc();
  setup_encoder();
}

void loop( ) {
  check_switch_condition();
  long_push_timer();
  release_timer();
  event_dispatcher();
  display_clock();
}

#define ON  true
#define OFF false
