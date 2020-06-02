/*
 *Check Button from the u8g2 libary 
 */ 
void check_button_event(void)
{
  Timer2.pause();
  if ( button_event == 0 ){
    button_event = u8g2.getMenuEvent();
  }
  processButtonEvent();
  Timer2.resume();
}
/*
 * Process the button event
 */
void processButtonEvent(){
  
  if ( button_event == U8X8_MSG_GPIO_MENU_NEXT){
    if(!Gauge_Active){
      switch( menuLevel ){
        case MAIN_MENU:
          menuNumber = menuNumber + 1;
          if(menuNumber > menuNumberMax ){
            menuNumber = 0;
          }
          break;
        case SEL_Gauge_Menu:
          GaugeNumber = GaugeNumber + 1;
          if(GaugeNumber > 4 ){
            GaugeNumber = 1;
          }
          break;
        default:
          break;  
      }
    }else{
      Selected_PID_Number = Selected_PID_Number + 1;
      if(Selected_PID_Number > 134){
        Selected_PID_Number = 1;        
      }
    }
      NoButtonActiveTime = millis();
      button_event = 0;
  }
  if ( button_event == U8X8_MSG_GPIO_MENU_PREV ){
    if(!Gauge_Active){
      switch(menuLevel){
        case MAIN_MENU:
          menuNumber = menuNumber - 1;
          if(menuNumber < 0  ){
             menuNumber = menuNumberMax;
          }
          break;
        case SEL_Gauge_Menu:
          GaugeNumber = GaugeNumber - 1;
          if(GaugeNumber < 1 ){
            GaugeNumber = 4;
          }
          break;
        default:
          break;  
      }
    }else{
      Selected_PID_Number = Selected_PID_Number - 1;
      if( Selected_PID_Number < 1 ){
        Selected_PID_Number = 134;        
      }
    }
    NoButtonActiveTime = millis();
    button_event = 0;
  }
  if(( button_event == U8X8_MSG_GPIO_MENU_SELECT )&&( !Gauge_Active )){
    Gauge_Active = true;
    button_event = 0;
    NoButtonActiveTime = millis();
  }
  if( ( button_event == 0 ) && ( ( millis()- NoButtonActiveTime ) > NoButtonActiveTimeout ) )
  {
    GaugeNumber = 0;
  }
}
