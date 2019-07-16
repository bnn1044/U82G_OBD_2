#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define ButtonUpdateRate_timer2  10000    //in mills
#define DisplauUpdateRate_timer3 250000    //in mills

const int numOfInputs = 3;
int lastInputState[numOfInputs] = {HIGH,HIGH,HIGH};
const int inputPins[numOfInputs] = {PB14,PB13,PB12};
int inputState[numOfInputs];
bool inputFlags[numOfInputs] = {HIGH,HIGH,HIGH};
long lastDebounceTime[numOfInputs] = {0,0,0};
long debounceDelay = 5;

struct menu_entry_type
{
  const uint8_t *font;
  uint16_t icon;
  const char *name;
};
struct menu_state
{
  int16_t menu_start;		    /* in pixel */
  int16_t frame_position;		/* in pixel */
  uint8_t position;			    /* position, array index */
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
struct menu_entry_type menu_entry_list[] =
{
  { u8g2_font_open_iconic_all_4x_t, 141, "GAUGES"},
  { u8g2_font_open_iconic_all_4x_t, 207, "SEARCH PID"},
  { u8g2_font_open_iconic_all_4x_t, 123, "0-60 Time"},
  //{ u8g2_font_open_iconic_all_4x_t, 184, "HOME"},
  { u8g2_font_open_iconic_all_4x_t, 282, "SETTING"},
  { NULL, 0, NULL } 
};
int8_t button_event = 0;		// set this to 0, once the event has been processed
int8_t preview_State;
float count;
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
  for( i = 0; i < 6; i++ )
  {
    r |= towards_int16( &(current->frame_position), destination->frame_position);
    r |= towards_int16( &(current->menu_start), destination->menu_start);
  }
  return r;
}
struct menu_state current_state = { ICON_BGAP, ICON_BGAP, 0 };
struct menu_state destination_state = { ICON_BGAP, ICON_BGAP, 0 };
void setup(void) {
 // u8g2.begin(/* menu_select_pin= */ PB13, /* menu_next_pin= */ PB12, /* menu_prev_pin= */ PB14);
  u8g2.begin();
  u8g2.setFont(u8g2_font_saikyosansbold8_8u);
  Serial.begin(38400);
  SetupTimer2();    //update input button
  SetupTimer3();   //update the display
  initialButton();
}
int Forward = 1;
void loop(void) {
  setInputFlags();
  /*if( Forward ==1 ){
    count ++;
  }else{
    count --;
    if(count <=0){
      Forward = 1;
    }
  }
  if ((count >=255) &&( Forward  ==1 )){
    Forward = 0;
  }
  Serial.println(count*count-2*4*count,2);
  delay(5);*/
}
void ReadButton(void){
 Timer2.pause();
 button_event = u8g2.getMenuEvent();
 if ( inputFlags[2] == LOW ){to_right(&destination_state);inputFlags[2] = HIGH;}
 if ( inputFlags[0] == LOW ){to_left(&destination_state);inputFlags[0] = HIGH;}
 if ( inputFlags[1] == LOW ){
   inputFlags[1] = HIGH;
   /*u8g2.setFont(u8g2_font_helvB10_tr );  
   u8g2.userInterfaceMessage("Selection:", menu_entry_list[destination_state.position].name, "", " Ok ");*/
  }
 Serial.println(current_state.position);
 Timer2.resume();
}

void UpdateDisplay(void){
   Timer3.pause();
   do{
      u8g2.firstPage();
      do
     {
       draw(&current_state);  
       u8g2.setFont(u8g2_font_helvB10_tr);  
       u8g2.setCursor((u8g2.getDisplayWidth()-u8g2.getStrWidth(menu_entry_list[destination_state.position].name))/2,u8g2.getDisplayHeight()-5);
       u8g2.print(menu_entry_list[destination_state.position].name);
     }while( u8g2.nextPage() );
   } while ( towards( &current_state, &destination_state ) );
   Timer3.resume();
}
/*
 * setup timer2 for reflash the button
 */
void SetupTimer2(){
  // Pause the timer while we're configuring it
  Timer2.pause();
  Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  Timer2.setPeriod(ButtonUpdateRate_timer2);          // in microseconds
  Timer2.setCompare(TIMER_CH1, 1);                    // overflow might be small
  Timer2.refresh();
  Timer2.attachInterrupt(TIMER_CH1,ReadButton);
  Timer2.resume();
}
void SetupTimer3(){
  Timer3.pause();
  Timer3.setMode(TIMER_CH1,TIMER_OUTPUT_COMPARE);
  Timer3.setPeriod(DisplauUpdateRate_timer3);                       // in microSecond
  Timer3.refresh();
  Timer3.setCompare(TIMER_CH1, 1);      // overflow might be small
  Timer3.attachInterrupt(TIMER_CH1,UpdateDisplay);
  Timer3.resume();
}
void initialButton(){
  for(int i = 0; i < numOfInputs; i++) {
    pinMode(inputPins[i], INPUT_PULLUP);
  }
}
void setInputFlags() {
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
 for(int i = 0; i < numOfInputs; i++){
  Serial.print(inputFlags[i]);
  Serial.print(" ");
 }
 Serial.println(" ");
}
/*void resolveInputFlags() {
 for(int i = 0; i < numOfInputs; i++) {
  if(inputFlags[i] == LOW) { 
    inputAction(i); 
    inputFlags[i] = LOW;
  } 
 } 
}
void inputAction(int input) {
 if(input == 0) {
   if (currentScreen == 0) {
      currentScreen = numOfScreens-1;
   }else{
      currentScreen--;
   }
 }else if( input == 1 ) {
   if (currentScreen == numOfScreens-1) {
      currentScreen = 0;
   }else{
      currentScreen++;
   }
 }else if(input == 2) {
   parameterChange(0);
 }
}*/
