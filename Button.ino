/*
 *Check Button from the u8g2 libary 
 */ 
#define FavouriteButtonTimeout 800   //ms

long FavouriteButtonTimer;

void check_button_event(void)
{
  Timer2.pause();
  if ( button_event == 0 ){
    button_event = u8g2.getMenuEvent();
  }
  Serial.println(button_event);
  processButtonEven();
   Timer2.resume();
}
/*
 * Process the button even
 */
void processButtonEven(){
 if ( ( button_event == U8X8_MSG_GPIO_MENU_NEXT )&&( Menu_Active ) ){to_right(&destination_state); NoButtonActiveTime = millis();button_event = 0;}
 if ( ( button_event == U8X8_MSG_GPIO_MENU_PREV )&&( Menu_Active ) ){to_left(&destination_state); NoButtonActiveTime = millis(); button_event = 0;}
 if ( button_event == U8X8_MSG_GPIO_MENU_SELECT ){
   if (not Menu_Active){ 
    Menu_Active = true; 
   }
   NoButtonActiveTime = millis();
   if ( destination_state.position == 5 ){  //exit
      Menu_Active = false;
      current_state = { ICON_BGAP, ICON_BGAP, 0 };
      destination_state = { ICON_BGAP, ICON_BGAP, 0 };
   }
   button_event = 0;
  }
   if ( (( millis()- FavouriteButtonTimer) > FavouriteButtonTimeout ) &&( Menu_Favourite ) ) {
   Menu_Favourite = false;
   FavouriteButtonTimer = 0;
 }
  if( ( button_event == 0 ) &&  ( ( millis()- NoButtonActiveTime ) > NoButtonActiveTimeout ) )
  {
    Menu_Active = false;
  }
  
}
