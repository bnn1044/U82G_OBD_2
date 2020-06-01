#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <SPI.h>
#include "ELM327.h"
#define LED_BANK         GPIOC
#define LED_PIN          13
#define LED_ON_STATE     1

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
Elm327 Elm;
int8_t button_event = 0;    // set this to 0, once the event has been processed
#define ButtonUpdateRate_timer2  10000    //in mills
long NoButtonActiveTime = 0;
long NoButtonActiveTimeout = 5000; 
long preview_time; 
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
#define menuNumberMax 3

boolean Menu_Active = false;
int menuNumber = 0;
struct menu_entry_type
{
  const uint8_t *font;
  uint16_t icon;
  const char *name;
};

struct menu_entry_type menu_entry_list[] =
{
  { u8g2_font_open_iconic_all_4x_t, 141, "4 GAUGES"},
  { u8g2_font_open_iconic_all_4x_t, 141, "SINGLE GAUGE"},
  { u8g2_font_open_iconic_all_4x_t, 207, "SEARCH PID"},
  { u8g2_font_open_iconic_all_4x_t, 123, "0-60 Time"},
};
void setup(void) {
  Serial.begin(38400);
  pinMode(PC13,OUTPUT);
  pinMode(PB13,OUTPUT);
  digitalWrite(PC13,HIGH);  
  u8g2.begin(/*menu select*/PB13, /*menu_prev*/PB14,/*menu_next*/PB12);
  while((Elm.begin() != ELM_SUCCESS)&&( digitalRead(PB13) ) ){
      displayMsg("INITIALIZE OBD");
  }
  SetupTimer2();    //update input button
}
void loop(void) {
   UpdateDisplay(menuNumber);
}
void Timer4_handler(){
  strobePin(PC13,2,250);
}
/*
 * pin for LED pin
 * count how many times
 * rate is the frequency in ms
 */
void strobePin(unsigned int pin, long count, long rate){
    long  c;
    while (count-- > 0)
    {
        for (c = rate; c > 0; c--)
        {
            delay(1);
        }
        digitalWrite(pin,HIGH);

        for (c = rate; c > 0; c--)
        {
            delay(1);
        }
        digitalWrite(pin,LOW);
    } 
}
