int FavouritePid_Dis_timeOut;
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
  }else{                                                        // display Gauge
    u8g2.clearBuffer();
    //disPlay_PID(FavouritePID_List[FavouritePID].PID_Number,FavouritePID_List[FavouritePID].name);
                 //COOLANT                ITA                    //RPM             BOOST
    display4PIDs(FavouritePID_List[0],FavouritePID_List[1],FavouritePID_List[2],FavouritePID_List[3]);
    u8g2.sendBuffer();
  }
   Timer3.resume();
}
void disPlay_PID(int pid,const char *name){
  int tempData;
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
    //Serial.println(FavouritePID);
  }
  
}
void display4PIDs(struct pid_name PID1,struct pid_name PID2,struct pid_name PID3,struct pid_name PID4){
  int data1,data2,data3,data4;
   u8g2.setFont(u8g2_font_saikyosansbold8_8u);
   u8g2.setCursor(0,7);                                                          //left top corner
   u8g2.print(PID1.name);
   u8g2.setCursor(u8g2.getDisplayWidth()- u8g2.getStrWidth( PID2.name ),7);     //Right top corner
   u8g2.print(PID2.name);
   u8g2.setCursor(0,39);                                                          //left bottom corner
   u8g2.print(PID3.name);                                   
   u8g2.setCursor(u8g2.getDisplayWidth()- u8g2.getStrWidth( PID4.name ),39);                                                          //left bottom corner
   u8g2.print(PID4.name);   
     
   obd.read(PID1.PID_Number,data1);
   u8g2.setFont(u8g2_font_timB24_tn );
   u8g2.setCursor(0,31);                                                           //left top corner
   u8g2.print(data1);

   Serial.print("data 2 : ");
   obd.read(PID2.PID_Number,data2);
   u8g2.setCursor(u8g2.getDisplayWidth()- u8g2.getStrWidth("9999"),31);
   if(data2 < 10){
    u8g2.setCursor(u8g2.getDisplayWidth()- u8g2.getStrWidth("9"),31);
   }else if(data2 < 100){
     u8g2.setCursor(u8g2.getDisplayWidth()- u8g2.getStrWidth("99"),31);
   }else if( data2 < 1000 ){
     u8g2.setCursor(u8g2.getDisplayWidth()- u8g2.getStrWidth("999"),31);
   }
   u8g2.print(data2);
   Serial.println(data2);
   
   obd.read(PID3.PID_Number,data3);
   u8g2.setCursor(0,63);                                                           
   u8g2.print(data3);


   Serial.print("data4 : ");
   obd.read(PID4.PID_Number,data4);  
   u8g2.setCursor(u8g2.getDisplayWidth()- u8g2.getStrWidth("9999"),63);
   if(data4 < 10 ){
    u8g2.setCursor(u8g2.getDisplayWidth()- u8g2.getStrWidth("9"),63);
   }else if(data4 < 100){
     u8g2.setCursor(u8g2.getDisplayWidth()- u8g2.getStrWidth("99"),63);
   }else if( data4 < 1000 ){
     u8g2.setCursor(u8g2.getDisplayWidth()- u8g2.getStrWidth("999"),63);
   }                                                        
   u8g2.print(data4);
   Serial.println(data4);
}
