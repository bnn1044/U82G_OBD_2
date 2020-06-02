#include "U82g_OBD_2.h" 
void drawIcon(int menuNumber){
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
  pid_name OBDPID;
  Timer3.pause();
  if( Menu_Active ){    // display menu
    drawIcon(menuNumber);
  }else{
    u8g2.clearBuffer();
    switch ( menuNumber ) {
      case 0: 
          display4PIDs();
          break;
      case 1:
          displayMsg("SINGLE GAUGE");
          break;
      case 2:
          displaySearchPID();
          break;
      case 3:
          display0to60Time();
          break;
    }
    u8g2.sendBuffer();
  }
   Timer3.resume();
}
void display4PIDs(){
  float data1,data2,data3,data4;
  //Serial.println(obd.getState());
 /* if( obd.getState() == OBD_CONNECTED ){
     obd.read(PID1.PID_Number,data1);
     obd.read(PID2.PID_Number,data2);
     obd.read(PID3.PID_Number,data3);
     obd.read(PID4.PID_Number,data4);
  }
  else{*/
    data1 = data2 = data3 = data4 = 0;
  //}
   menuLevel = SEL_Gauge_Menu;
   switch (GaugeNumber){
      case 1 :              // top left corner selected
          u8g2.drawBox(0,10,30 ,24);
          break;
      case 2 :              // top left corner selected
          u8g2.drawBox(100,10,50 ,24);
          break;
      case 3 :              // top left corner selected
          u8g2.drawBox(0,45,50 ,24);
          break;
      case 4 :              // top left corner selected
          u8g2.drawBox(100,45,50 ,24);
      break;
   }
   if(!Gauge_Active){
      u8g2.setFont(u8g2_font_saikyosansbold8_8u);
      u8g2.setCursor(0,7);                                                          //left top corner
      u8g2.print(PID_List[5].name);

      u8g2.setCursor(u8g2.getDisplayWidth()- u8g2.getStrWidth(PID_List[111].name ),7);     //Right top corner
      u8g2.print(PID_List[111].name);

      u8g2.setCursor(0,39);                                                          //left bottom corner
      u8g2.print(PID_List[14].name);
                                    
      u8g2.setCursor(u8g2.getDisplayWidth()- u8g2.getStrWidth( PID_List[15].name ),39);                                                          //left bottom corner
      u8g2.print(PID_List[15].name);   
      
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
   }else if(Gauge_Active){
      u8g2.setFont(u8g2_font_tenthinguys_t_all  );
      u8g2.setCursor(5,26);  
      u8g2.print(PID_List[Selected_PID_Number].name); 
      Serial.println(Selected_PID_Number);
      Serial.println(PID_List[Selected_PID_Number].name); 
      Serial.println(u8g2.getStrWidth( PID_List[Selected_PID_Number].name ) );
   }
}
void displayMsg(char *msg){
   u8g2.clearBuffer();
   u8g2.setFont(u8g2_font_pxplustandynewtv_8f);
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
   byte Speed;
   long int StartTimer;
   long int FinishTimer;
   char* msg;
   boolean TestOver;
   int status = Elm.vehicleSpeed(Speed);
   StartTimer = 0;
   u8g2.setFont(u8g2_font_saikyosansbold8_8u);
   u8g2.setCursor(0,7); 
   // check speed at 0
   do{
    status = Elm.vehicleSpeed(Speed);
    displayMsg("STOP FIRST!");
   } while( ( int( Speed ) > 0) );
   StartTimer = 0;
   do{
      status = Elm.vehicleSpeed(Speed);
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
      u8g2.setFont(u8g2_font_pxplustandynewtv_8f);
      u8g2.print(" S");
      u8g2.setCursor(20,63);
      u8g2.setFont(u8g2_font_timB24_tn );
      u8g2.print( Speed,2 );
      u8g2.setFont(u8g2_font_pxplustandynewtv_8f);
      u8g2.print(" M/H");
      u8g2.sendBuffer();
      if( digitalRead(PB13) == 0 ) {
        menuNumber = 0;
        TestOver = true;
        break;
      }
   }while( ( int( Speed ) < 60 )||( !TestOver ));
   TestOver = false;
}
void displaySearchPID(){
   char* buffer;
   char* temp;
   /*if( obd.getState() == OBD_CONNECTED ){
       obd.sendCommand("0100\r", buffer);
   }*/
   char *p = strstr(buffer,"41 00");
   if(p){
    temp[0] = buffer[6];
    temp[1] = buffer[7];
    temp[2] = buffer[9];
    temp[3] = buffer[10];
    temp[4] = buffer[12];
    temp[5] = buffer[13];
    temp[6] = buffer[15];
    temp[7] = buffer[16];
   }
   u8g2.clearBuffer();
   u8g2.setCursor(0,10);
   u8g2.setFont(u8g2_font_pxplustandynewtv_8f );
   u8g2.print("SEARCHING");
   u8g2.setCursor(0,20);
   u8g2.setFont(u8g2_font_pxplustandynewtv_8f );
   u8g2.print(temp);
   u8g2.sendBuffer();
}
void displaySinglePid(){
}
