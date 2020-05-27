/*
 * setup timer2 for reflash the button
 */
void SetupTimer2(){
  // Pause the timer while we're configuring it
  Timer2.pause();
  Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  Timer2.setPeriod(ButtonUpdateRate_timer2);          // in microseconds
  Timer2.setCompare(TIMER_CH1, 1);                    // overflow might be small
  Timer2.refresh();
  Timer2.attachInterrupt(TIMER_CH1,check_button_event);
  Timer2.resume();
}
void SetupTimer4(){
  Timer4.pause(); 
  Timer4.setPrescaleFactor(3600);     //72000000 / 3600 = 2000hz,  500us
  Timer4.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  Timer4.setCount(0);
  Timer4.setOverflow(30000);
  Timer4.setCompare(TIMER_CH1, 1000);   
  Timer4.attachInterrupt(TIMER_CH1, Timer4_handler);
  Timer4.refresh();
  Timer4.resume();
}
