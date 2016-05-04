#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>


#include "FastLED.h"
#define LED_PIN     9
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS    60

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 60
#define color_second Blue
#define color_minute Green
#define color_hour Red
byte color_second_R = 0;
byte color_second_G = 0;
byte color_second_B = 255;
byte color_minute_R = 0;
byte color_minute_G = 255;
byte color_minute_B = 0;
byte color_hour_R = 255;
byte color_hour_G = 0;
byte color_hour_B = 0;


CRGB leds[NUM_LEDS];

byte hour_;
byte minute_;
byte second_;
byte inside_hour;

bool error_sens = LOW;


unsigned long timer1, timer2;
bool reset_led[1];
unsigned int looping_error_time;

void setup() {

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  Serial.begin(115200);
  pinMode(13,OUTPUT);

}

void loop() {
  tmElements_t tm;
  if(reset_led[0] == LOW){ for(byte i = 0 ; i <= 60 ; i++){leds[i] = CRGB(0, 0, 0);} reset_led[0] = HIGH;}
    

  if (RTC.read(tm)) {
    error_sens = LOW;
    hour_ = tm.Hour;
    minute_ = tm.Minute;
    second_ = tm.Second;
    Serial.print("   ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();

  }
  else {
    error_sens = HIGH;
    Serial.println(error_sens);
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }

  }

  if(error_sens == LOW) control_led();
  else error_control_led();
}

void control_led(){
  for(byte white_show = 0 ; white_show <= 55 ; white_show +=5){
    leds[white_show] = CRGB(255, 255, 255);
  }
    //second led control BEGIN
    leds[second_] = CRGB(color_second_R, color_second_G, color_second_B);
    if(second_ == 0 && minute_ == 59) leds[59] = CRGB(color_minute_R, color_minute_G, color_minute_B);
    else{if(second_ == 0 && inside_hour == 59) leds[59] = CRGB(color_hour_R, color_hour_G, color_hour_B);
    else{ if(second_ == 0) leds[59] = CRGB(0, 0, 0);
    else{
    if(second_ - 1 == minute_) leds[second_ - 1] = CRGB(color_minute_R, color_minute_G, color_minute_B);
    else{
    if(second_ - 1 == inside_hour) leds[second_ - 1] = CRGB(color_hour_R, color_hour_G, color_hour_B);
    else{
    if(second_ - 1  ==  5 || second_ - 1  ==  10 || second_ - 1  ==  15 || second_ - 1  ==  20 || second_ - 1  ==  25 || second_ - 1  ==  30 || second_ - 1  ==  35 || second_ - 1  ==  40 || second_ - 1  ==  45 || second_ - 1  ==  50 || second_ - 1  ==  55 || second_ - 1  ==  0){
    leds[second_ - 1] = CRGB(255, 255, 255);
   }
   else{leds[second_ - 1] = CRGB(0, 0, 0);}
}
}
}
}
}
    //second led control END

    //miniute led control BEGIN
    leds[minute_] = CRGB(color_minute_R, color_minute_G, color_minute_B);
    if(minute_ == 0 && second_ == 59) leds[59] = CRGB(color_second_R, color_second_G, color_second_B);
    else{if(minute_ == 0 && inside_hour == 59) leds[59] = CRGB(color_hour_R, color_hour_G, color_hour_B);
    else{
    if(minute_ == 0) leds[59] = CRGB(0, 0, 0);
    else{
    if(minute_ - 1 == second_)leds[minute_ - 1] = CRGB(color_second_R, color_second_G, color_second_B);
    else{
    if(minute_ - 1 == inside_hour) leds[minute_ - 1] = CRGB(color_hour_R, color_hour_G, color_hour_B);
    else{
    if(minute_ - 1  ==  5 || minute_ - 1  ==  10 || minute_ - 1  ==  15 || minute_ - 1  ==  20 || minute_ - 1  ==  25 || minute_ - 1  ==  30 || minute_ - 1  ==  35 || minute_ - 1  ==  40 || minute_ - 1  ==  45 || minute_ - 1  ==  50 || minute_ - 1  ==  55 || minute_ - 1  ==  0){
    leds[minute_ - 1] = CRGB(255, 255, 255);
   }
   else{leds[minute_ - 1] = CRGB(0, 0, 0);}
}
}
}
}
}
    //minute led contrl END

    //hour led control BEGIN
    if(hour_ >= 12) hour_ -= 12;
    inside_hour = hour_ * 5 + minute_ / 12;

    leds[inside_hour] = CRGB(color_hour_R, color_hour_G, color_hour_B);

    if(inside_hour == 0 && second_ == 59) leds[59] = CRGB(color_second_R, color_second_G, color_second_B);
    else{if(inside_hour == 0 && minute_ == 59) leds[59] = CRGB(color_minute_R, color_minute_G, color_minute_B);
    else{
    if(inside_hour == 0) leds[59] = CRGB(0, 0, 0);
    else{
    if(inside_hour - 1 == second_)leds[inside_hour - 1] = CRGB(color_second_R, color_second_G, color_second_B);
    else{
    if(inside_hour - 1 == minute_) leds[inside_hour - 1] = CRGB(color_minute_R, color_minute_G, color_minute_B);
    else{
    if(inside_hour - 1  ==  5 || inside_hour - 1  ==  10 || inside_hour - 1  ==  15 || inside_hour - 1  ==  20 || inside_hour - 1  ==  25 || inside_hour - 1  ==  30 || inside_hour - 1  ==  35 || inside_hour - 1  ==  40 || inside_hour - 1  ==  45 || inside_hour - 1  ==  50 || inside_hour - 1  ==  55 || inside_hour - 1  ==  0){
    leds[inside_hour - 1] = CRGB(255, 255, 255);
   }
   else{leds[inside_hour - 1] = CRGB(0, 0, 0);}
}
}
}
}
}
    

    //hour led control END

    //cover led control BEGIN
    if(second_ == minute_){
      leds[second_] = CRGB(color_second_R, color_second_G, color_second_B);
    }
    if(second_ == inside_hour){
      leds[second_] = CRGB(color_second_R, color_second_G, color_second_B);
    }
    if(minute_ == inside_hour){
      leds[minute_] = CRGB(color_minute_R, color_minute_G, color_minute_B);
    }
    if(second_ == minute_ && second_ == hour_){
        leds[second_] = CRGB(color_second_R, color_second_G, color_second_B);
    }
    //cover led control END


  FastLED.show(); 
}

void error_control_led(){
  reset_led[0] = LOW;

    if(millis() - timer1 >= looping_error_time){
      for(byte i = 0 ; i <= 60 ; i++){
      for(byte x = 0 ; x <= 100 ; x++){
        for(byte y = 0 ; y <= 5 ; x++){
      leds[i] = CRGB(255, 0, 0);
      leds[i + y] = CRGB(255 + x, 0, 0);
      leds[i - y] = CRGB(255 - x, 0, 0);
      FastLED.show();
      for(looping_error_time = 0 ; looping_error_time <= 30000 ; looping_error_time += 1){}
      for(byte position = 0 ; position <= 60 ; position ++){
        leds[position] = CRGB(0, 255, 0);
        FastLED.show();
      }
      FastLED.show();
      
    }
  }
}
    
  }
    
}


void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}



