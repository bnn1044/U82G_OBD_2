#include "U82g_OBD_2.h" 
void draw(struct menu_state *state)
{
  int16_t x;
  uint8_t i;
  x = state->menu_start;
  i = 0;
  while( menu_entry_list[i].icon > 0 )  
  {
    if ( x >= -ICON_WIDTH && x < u8g2.getDisplayWidth() )
    {
      u8g2.setFont(menu_entry_list[i].font);
      u8g2.drawGlyph(x, ICON_Y, menu_entry_list[i].icon );
    }
    i++;
    x += ICON_WIDTH + ICON_GAP;
  }
  u8g2.drawFrame(state->frame_position-1, ICON_Y-ICON_HEIGHT-1, ICON_WIDTH+2, ICON_WIDTH+2);
  u8g2.drawFrame(state->frame_position-2, ICON_Y-ICON_HEIGHT-2, ICON_WIDTH+4, ICON_WIDTH+4);
  u8g2.drawFrame(state->frame_position-3, ICON_Y-ICON_HEIGHT-3, ICON_WIDTH+6, ICON_WIDTH+6);
}
void to_right(struct menu_state *state)
{
  if ( menu_entry_list[state->position+1].font != NULL )
  {
    if ( (int16_t)state->frame_position+ 2*(int16_t)ICON_WIDTH + (int16_t)ICON_BGAP < (int16_t)u8g2.getDisplayWidth() )
    {
      state->position++;
      state->frame_position += ICON_WIDTH + (int16_t)ICON_GAP;
    }
    else
    {
      state->position++;      
      state->frame_position = (int16_t)u8g2.getDisplayWidth() - (int16_t)ICON_WIDTH - (int16_t)ICON_BGAP;
      state->menu_start = state->frame_position - state->position*((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);
    }
  }
}
void to_left(struct menu_state *state)
{
  if ( state->position > 0 )
  {
    if ( (int16_t)state->frame_position >= (int16_t)ICON_BGAP+(int16_t)ICON_WIDTH+ (int16_t)ICON_GAP )
    {
      state->position--;
      state->frame_position -= ICON_WIDTH + (int16_t)ICON_GAP;
    }    
    else
    {
      state->position--; 
      state->frame_position = ICON_BGAP;
      state->menu_start = state->frame_position - state->position*((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);      
    }
  }
}
uint8_t towards_int16(int16_t *current, int16_t dest)
{
  if ( *current < dest )
  {
    (*current)++;
    return 1;
  }
  else if ( *current > dest )
  {
    (*current)--;
    return 1;
  }
  return 0;
}
uint8_t towards(struct menu_state *current, struct menu_state *destination)
{
  uint8_t r = 0;
  uint8_t i;
  for( i = 0; i < 7; i++ )
  {
    r |= towards_int16( &(current->frame_position), destination->frame_position);
    r |= towards_int16( &(current->menu_start), destination->menu_start);
  }
  return r;
}
/*
 * interrupt handlar for the timer 3
 * 
 */
void UpdateDisplay(void){
  int temdata;
  Timer3.pause();
  if(Menu_Active){    // display menu
    do{
       u8g2.clearBuffer();
       draw(&current_state);  
       u8g2.setFont(u8g2_font_helvB10_tr);  
       u8g2.setCursor((u8g2.getDisplayWidth()-u8g2.getStrWidth(menu_entry_list[destination_state.position].name))/2,u8g2.getDisplayHeight()-5);
       u8g2.print(menu_entry_list[destination_state.position].name);
       u8g2.sendBuffer();
    }while ( towards( &current_state, &destination_state ) );
  }else{
    u8g2.clearBuffer();
    Serial.println(destination_state.position);
    switch ( destination_state.position ) {
      case 0: 
          display4PIDs(PID_List[5],PID_List[112],
                       PID_List[14],PID_List[119]);
          break;
      case 1:
          displayDebug("FAVOURITE");
          break;
      case 2:
          displayDebug("SEARCH PID");
          break;
      case 3:
          display0to60Time();
          break;
      case 4:
          displayDebug("SETTING");
          break;
    }
    u8g2.sendBuffer();
  }
   Timer3.resume();
}
void disPlay_PID(int pid,const char *name){
  float tempData;
  u8g2.setFont(u8g2_font_helvB10_tr);  
  u8g2.setCursor(( u8g2.getDisplayWidth()- u8g2.getStrWidth(name))/2 , 14);
  u8g2.print(name);                                                           //active_element.name);
  u8g2.print(' ');                                                            //active_element.name);
  if( not Menu_Favourite ){
    u8g2.setFont( u8g2_font_fub35_tr ); 
    obd.read(pid,tempData);
    u8g2.setCursor( ( u8g2.getDisplayWidth()- (u8g2.getStrWidth("9999") ) ) / 2 ,u8g2.getDisplayHeight() - 2);
    u8g2.print(tempData);
  }else{
    
  }
}
void display4PIDs(struct pid_name PID1,struct pid_name PID2,struct pid_name PID3,struct pid_name PID4){
  float data1,data2,data3,data4;
  //Serial.println(obd.getState());
  if( obd.getState() == OBD_CONNECTED ){
     obd.read(PID1.PID_Number,data1);
     obd.read(PID2.PID_Number,data2);
     obd.read(PID3.PID_Number,data3);
     obd.read(PID4.PID_Number,data4);
  }
  else{
    data1 = data2 = data3 = data4 = 0;
  }
   u8g2.setFont(u8g2_font_saikyosansbold8_8u);
   u8g2.setCursor(0,7);                                                          //left top corner
   u8g2.print(PID1.name);
   u8g2.setCursor(u8g2.getDisplayWidth()- u8g2.getStrWidth( PID2.name ),7);     //Right top corner
   u8g2.print(PID2.name);
   u8g2.setCursor(0,39);                                                          //left bottom corner
   u8g2.print(PID3.name);                                   
   u8g2.setCursor(u8g2.getDisplayWidth()- u8g2.getStrWidth( PID4.name ),39);                                                          //left bottom corner
   u8g2.print(PID4.name);   
   //left top corner 
   u8g2.setFont(u8g2_font_timB24_tn );
   u8g2.setCursor(0,31);                                                           
   u8g2.print( data1,0 );
   //right top corner
   
   u8g2.setCursor(u8g2.getDisplayWidth()- setCursorUseNumber(data2),31);
   u8g2.print( data2,1 );
   
   //left bot corner
   u8g2.setCursor(0,63);                                                           
   u8g2.print(data3,0);
   
   //Righft bot corner
   u8g2.setCursor(u8g2.getDisplayWidth()- setCursorUseNumber(data4),63);
   u8g2.setCursor(85,63);
   u8g2.print(data4,0);
   
}
void displayDebug(char *msg){
   u8g2.clearBuffer();
   u8g2.setFont(u8g2_font_7x14B_tf  );
   u8g2.setCursor(( u8g2.getDisplayWidth()- u8g2.getStrWidth(msg))/2,u8g2.getDisplayHeight()-20);
   u8g2.print(msg);
   u8g2.sendBuffer();
}
int setCursorUseNumber(float number){
  int numberWidth;
  if((number < 0 )&&( number >= -10)){
    numberWidth =  u8g2.getStrWidth("-9.9");
  }
  if((number < -10 )&&( number >= -100)){
    numberWidth =  u8g2.getStrWidth("-99.9");
  }
  if((number < -10 )&&( number >= -100)){
    numberWidth =  u8g2.getStrWidth("-99.9");
  }
  if((number < 10 )&&( number >= 0)){
    numberWidth =  u8g2.getStrWidth("9.9");
  }
  if((number < 100 )&&( number >= 10)){
    numberWidth =  u8g2.getStrWidth("99.9");
  }
  if( (number < 1000 )&&( number >= 100) ){
    numberWidth =  u8g2.getStrWidth("999.9");
  }
  if( (number < 10000 )&&( number >= 1000) ){
    numberWidth =  u8g2.getStrWidth("9999.9");
  }
  return numberWidth;
}
void display0to60Time(){
   float Speed;
   long int StartTimer;
   long int FinishTimer;
   StartTimer = 0;
   u8g2.setFont(u8g2_font_saikyosansbold8_8u);
   u8g2.setCursor(0,7); 
   Speed = 0;
  /*if( obd.getState() == OBD_CONNECTED ){
   obd.read(PID_SPEED,Speed);
  }*/
   // wait for speed =  0
   while( Speed > 0){
    if(anyButtonPress()){
      Menu_Active = true;
      break;
    }
    displayDebug("Stop First");
    Serial.println( "Stop First" );
   }
   displayDebug("Ready ! ");
   while(Speed < 60 ){
      if( anyButtonPress() ){
         Menu_Active = true;
         break;
      }
      /*if( obd.getState() == OBD_CONNECTED ){
        obd.read(PID_SPEED,Speed);
      }*/
      if( Speed > 0 ){
        StartTimer = millis();
      }
      displayDebug("Waiting for speed");
      Serial.println( "Waiting for speed" );
      strobePin(PC13,2,100);
      delay(50);
   }
   FinishTimer = millis() - StartTimer;
}
