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
  Timer2.attachInterrupt(TIMER_CH1,ReadButton);
  Timer2.resume();
}
