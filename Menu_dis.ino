#include "U82g_OBD_2.h" 
void drawIcon(int menuNumber)
{
   u8g2.clearBuffer();
   u8g2.setFont(menu_entry_list[menuNumber].font);
   u8g2.drawGlyph( ( ( 128 - ICON_WIDTH )/2 ), ICON_Y, menu_entry_list[menuNumber].icon );
   u8g2.setFont(u8g2_font_helvB10_tr);  
   u8g2.setCursor((u8g2.getDisplayWidth()-u8g2.getStrWidth(menu_entry_list[menuNumber].name))/2,u8g2.getDisplayHeight()-5);
   u8g2.print(menu_entry_list[menuNumber].name);
   u8g2.sendBuffer();
}
/*
 * interrupt handlar for the timer 3
 */
void UpdateDisplay(int menuNumber){
  int temdata;
  Timer3.pause();
  //Serial.println(Menu_Active);
  if( Menu_Active ){    // display menu
    drawIcon(menuNumber);
  }else{
    u8g2.clearBuffer();
    switch ( menuNumber ) {
      case 0: 
          display4PIDs(PID_List[5],PID_List[112],PID_List[14],PID_List[119]);
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
   u8g2.setFont(u8g2_font_saikyosansbold8_8u);
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
   char* msg;
   if( obd.getState() == OBD_CONNECTED ){
     obd.read(PID_SPEED,Speed);
   }
   StartTimer = 0;
   u8g2.setFont(u8g2_font_saikyosansbold8_8u);
   u8g2.setCursor(0,7); 
   // check speed at 0
   do{
    if( obd.getState() == OBD_CONNECTED ){
        obd.read(PID_SPEED,Speed);
    }
    displayDebug("STOP FIRST!");
   } while( ( int( Speed ) > 0) );
   // check if speed >60;
   StartTimer = 0;
   do{
      if( obd.getState() == OBD_CONNECTED ){
          obd.read(PID_SPEED,Speed);
      }
      if( ( int( Speed )== 0 )){
        StartTimer = millis();
        FinishTimer = millis() - StartTimer;   
      }else{
        FinishTimer = millis() - StartTimer;
      }
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_timB24_tn );
      u8g2.setCursor(20,31);
      u8g2.print( FinishTimer /1000.0 , 2 );
      u8g2.setFont(u8g2_font_saikyosansbold8_8u);
      u8g2.print(" S");
      u8g2.setCursor(20,63);
      u8g2.setFont(u8g2_font_timB24_tn );
      u8g2.print( Speed,2 );
      u8g2.setFont(u8g2_font_saikyosansbold8_8u);
      u8g2.print(" M/H");
      u8g2.sendBuffer();
   }while( ( int( Speed ) < 60 ));
}
void displaySearchPID(){
  
}
