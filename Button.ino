long FavouriteButtonTimer;
#define FavouriteButtonTimeout 800   //ms
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
 handleMenuButton();
 handleFavouriteButton();
 Timer2.resume();
}
void handleFavouriteButton(){
 if (( inputFlags[2] == LOW )&&( not Menu_Active )){
   Menu_Favourite = true;
   inputFlags[2] = HIGH;
    FavouritePID ++;
    if ( FavouritePID >= FavouritePID_Max){
      FavouritePID = 0;
    }
    FavouriteButtonTimer = millis();
 }
 if (( inputFlags[0] == LOW )&&( not Menu_Active )){
    inputFlags[0] = HIGH;
    Menu_Favourite = true;
    FavouritePID--;
    if ( FavouritePID < 0){
      FavouritePID = FavouritePID_Max;
    }
    FavouriteButtonTimer = millis();
 }
 if ( (( millis()- FavouriteButtonTimer) > FavouriteButtonTimeout ) &&( Menu_Favourite ) ) {
   Menu_Favourite = false;
   FavouriteButtonTimer = 0;
 }
}

void handleMenuButton(){
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
      current_state = { ICON_BGAP, ICON_BGAP, 0 };
      destination_state = { ICON_BGAP, ICON_BGAP, 0 };
   }
   /*switch(destination_state.position){
    case 0:
      //display4PIDs(struct pid_name PID1,struct pid_name PID2,struct pid_name PID3,struct pid_name PID4);
      break;
   }*/
  }
  
  if( ( inputFlags[0] == HIGH) && 
      ( inputFlags[1] == HIGH ) && 
      ( inputFlags[2] == HIGH )&& 
      ( ( millis()- NoButtonActiveTime ) > NoButtonActiveTimeout ) )
  {
    Menu_Active = false;
  }
  
}
