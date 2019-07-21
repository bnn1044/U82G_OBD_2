#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <SPI.h>
#include "OBD.h"

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
COBD obd;

#define ButtonUpdateRate_timer2  50000    //in mills
//#define DisplauUpdateRate_timer3 200000    //in mills

const int numOfInputs = 3;
int lastInputState[numOfInputs] = {HIGH,HIGH,HIGH};
const int inputPins[numOfInputs] = {PB14,PB13,PB12};
int inputState[numOfInputs];
bool inputFlags[numOfInputs] = {HIGH,HIGH,HIGH};
long lastDebounceTime[numOfInputs] = {0,0,0};
long debounceDelay = 5;
long NoButtonActiveTime = 0;
long NoButtonActiveTimeout = 5000; 
long preview_time; 
//variable for setting up favourate

#define FavouritePID_Max           10
int FavouritePID = 0;
boolean Menu_Favourite;
struct pid_name{
  int16_t PID_Number;
  const char *name;
};
struct pid_name FavouritePID_List[10] = {
  {PID_COOLANT_TEMP,"COOLANT"},   //0
  {PID_CHARGE_AIR_TEMP,"ITA"},  //1
  {PID_RPM,"RPM"},                //2
  {PID_BOOST_CONTROL,"BOOST"},                  //5
};
/*
  Icon configuration
  Width and height must match the icon font size
  GAP: Space between the icons
  BGAP: Gap between the display border and the cursor.
*/
#define ICON_WIDTH 32
#define ICON_HEIGHT 32
#define ICON_GAP 4
#define ICON_BGAP 16
#define ICON_Y 32+ ICON_GAP
boolean Menu_Active = false;

struct active_element
{
  const char *name;       
  int16_t pid;
  const char *unit;
};
struct menu_entry_type
{
  const uint8_t *font;
  uint16_t icon;
  const char *name;
};
struct menu_state
{
  int16_t menu_start;        /* in pixel */
  int16_t frame_position;   /* in pixel */
  uint8_t position;         /* position, array index */
};

struct menu_entry_type menu_entry_list[] =
{
  { u8g2_font_open_iconic_all_4x_t, 141, "GAUGES"},
  { u8g2_font_open_iconic_all_4x_t, 183, "FAVOURITE"},
  { u8g2_font_open_iconic_all_4x_t, 207, "SEARCH PID"},
  { u8g2_font_open_iconic_all_4x_t, 123, "0-60 Time"},
  { u8g2_font_open_iconic_all_4x_t, 282, "SETTING"},
  { u8g2_font_open_iconic_all_4x_t, 65, "EXIT"},
  { NULL, 0, NULL } 
};
struct menu_state current_state = { ICON_BGAP, ICON_BGAP, 0 };
struct menu_state destination_state = { ICON_BGAP, ICON_BGAP, 0 };

void setup(void) {
  Serial.begin(38400);
  obd.begin(); 
  pinMode(PC13,OUTPUT);
  digitalWrite(PC13,HIGH);
  initialButton();  
  u8g2.begin();
  while (!obd.init()){
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_helvB10_tr);  
      u8g2.setCursor(( u8g2.getDisplayWidth()- u8g2.getStrWidth("WAIT FOR OBD"))/2,u8g2.getDisplayHeight()-20);
      u8g2.print("WAIT FOR OBD");
      u8g2.sendBuffer();
      digitalWrite(PC13,HIGH);
      delay(200);
      digitalWrite(PC13,LOW);
      delay(200);
  }
  SetupTimer2();    //update input button
}
void loop(void) {
   //Serial.println(millis() - preview_time);
 // if ( millis() - preview_time > 5 ){
   UpdateDisplay();
   //preview_time = millis();
   
 // }
}
