/*
 * initialize the hardware for button input
 */
void initialButton(){
  for(int i = 0; i < numOfInputs; i++) {
    pinMode(inputPins[i], INPUT_PULLUP);
  }
}
/*
 * Read the input button state from the interrupt
 */
void ReadButton_input() {
 for(int i = 0; i < numOfInputs; i++) { 
  int reading = digitalRead(inputPins[i]);
  if (reading != lastInputState[i]) { 
    lastDebounceTime[i] = millis(); 
  }
  if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading != inputState[i]) {
         inputState[i] = reading;
         if (inputState[i] == LOW) {
            inputFlags[i] = LOW;
         }
      }
  }
  lastInputState[i] = reading;
 }
}
/*
 * interrupt handlar for Timer 2
 */
void ReadButton(void){
 Timer2.pause();
 ReadButton_input();
 
 if ( ( inputFlags[2] == LOW )&&( Menu_Active ) ){to_right(&destination_state);inputFlags[2] = HIGH; NoButtonActiveTime = millis();}
 if ( ( inputFlags[0] == LOW )&&( Menu_Active ) ){to_left(&destination_state);inputFlags[0] = HIGH; NoButtonActiveTime = millis();}
 
 if ( inputFlags[1] == LOW ){
   if (not Menu_Active){ 
    Menu_Active = true; 
   }
   
   NoButtonActiveTime = millis();
   inputFlags[1] = HIGH;
   if ( destination_state.position == 5 ){  //exit
    Menu_Active = false;
   }
  }
  if( ( inputFlags[0] == HIGH) && 
      (inputFlags[1] == HIGH ) && 
      (inputFlags[2] == HIGH )&& 
      ( (millis()- NoButtonActiveTime) > NoButtonActiveTimeout))
  {
    Menu_Active = false;
  }
  
 Timer2.resume();
}
