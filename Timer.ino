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
/*
 * setup timer 3 for udpating the screen
 */
void SetupTimer3(){
  Timer3.pause();
  Timer3.setMode(TIMER_CH1,TIMER_OUTPUT_COMPARE);
  Timer3.setPeriod(DisplauUpdateRate_timer3);                       // in microSecond
  Timer3.refresh();
  Timer3.setCompare(TIMER_CH1, 1);      // overflow might be small
  Timer3.attachInterrupt(TIMER_CH1,UpdateDisplay);
  Timer3.resume();
}
