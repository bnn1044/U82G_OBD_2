/*
 *Check Button from the u8g2 libary 
 */ 
void check_button_event(void)
{
  Timer2.pause();
  if ( button_event == 0 ){
    button_event = u8g2.getMenuEvent();
  }
  Serial.println(button_event);
  processButtonEvent();
  Timer2.resume();
}
/*
 * Process the button event
 */
void processButtonEvent(){
  
  if ( button_event == U8X8_MSG_GPIO_MENU_NEXT){
    menuNumber = menuNumber + 1;
    if(menuNumber > menuNumberMax ){
      menuNumber = 0;
    }
    NoButtonActiveTime = millis();
    button_event = 0;
    Menu_Active = true;
  }
  if ( button_event == U8X8_MSG_GPIO_MENU_PREV ){
    menuNumber = menuNumber - 1;
    if(menuNumber < 0  ){
      menuNumber = menuNumberMax;
    }
    NoButtonActiveTime = millis();
    button_event = 0;
    Menu_Active = true;
  }
  if (( button_event == U8X8_MSG_GPIO_MENU_SELECT )&&( Menu_Active )){
    Menu_Active = false;
    button_event = 0;
  }else if(( button_event == U8X8_MSG_GPIO_MENU_SELECT )&&( !Menu_Active )){
    Menu_Active = true;
    button_event = 0;
    NoButtonActiveTime = millis();
  }
  if( ( button_event == 0 ) && ( Menu_Active )  && ( ( millis()- NoButtonActiveTime ) > NoButtonActiveTimeout ) )
  {
    Menu_Active = false;
  }
}
