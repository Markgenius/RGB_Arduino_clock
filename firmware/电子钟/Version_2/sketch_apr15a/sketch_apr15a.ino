#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>

#include <TimerOne.h>


#include "FastLED.h"
#define LED_PIN     9
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS    60

float brightness = 255;
#define sampletime 10
float kp = 0.01;
float kd = 0.01;
float speed;
float val;
unsigned long control_second_color_interrupt_;
byte control_white_light_changing = 200;
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
byte state_second = 255;
byte last_state_second, state_minute, last_state_minute, state_hour, last_state_hour;
//bool color_control_second;
int returner = 1;
int control_second_color;
int light_resistor;

int white_color_data;
int returner_white = 1;
int control_white_timer_inside;
int control_white_color;


CRGB leds[NUM_LEDS];

byte hour_;
byte minute_;
byte second_;
byte inside_hour;

bool error_sens = LOW;


unsigned long timer1, timer3;
float timer2;
bool reset_led[1];
unsigned int looping_error_time;

void setup() {

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( brightness );

  Timer1.initialize(2000);             // 5000us = 5ms
  Timer1.attachInterrupt(control_second_color_interrupt);

  Serial.begin(115200);
  pinMode(13,OUTPUT);
  pinMode(A0,INPUT_PULLUP);

}

void loop() {
 /* if(millis() - timer3 >= 500){
  light_resistor = analogRead(A0);
  if(light_resistor >= 400) light_resistor = 400;
  if(light_resistor <= 150) light_resistor = 150;

  brightness = map(light_resistor, 255, 10, 400, 150);
  FastLED.setBrightness(brightness);
  timer3 = millis();
}*/

  control_brightness();

  


  state_second = second_;
      if(state_second != last_state_second){
      color_control_second_resetor();
      last_state_second = state_second;
    }


  tmElements_t tm;
  if(reset_led[0] == LOW){ for(byte i = 0 ; i <= 60 ; i++){leds[i] = CRGB(0, 0, 0);} reset_led[0] = HIGH;}
    

  if (RTC.read(tm)) {
    error_sens = LOW;
    hour_ = tm.Hour;
    minute_ = tm.Minute;
    second_ = tm.Second;
    /*Serial.print("   ");
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
    Serial.print("  white_color_data   ");
    Serial.print(white_color_data);
    Serial.print(" returner_white  ");
    Serial.print(returner_white);
    Serial.print("  light_resistor   ");
    Serial.print(val);
    Serial.print("  speed   ");
    Serial.print(speed);
    Serial.print("         color_second_B:");
    Serial.println(color_second_B);*/


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


void control_white_color_interrupt(){
  /*if(control_white_light_changing <= 255) for(control_white_light_changing = 200 ; control_white_light_changing <= 255 ; control_white_light_changing ++){}
  else
  */
  /*byte color_white_inside;
  //if(millis() - timer2 >= 100){
  for(int i = 0 ; i <= 100 ; i++){
    if(i == 0) returner_white *= -1;
    if(returner_white == -1) color_white_inside = 255;
    if(returner_white == 1) color_white_inside = 155;
    white_color_data = color_white_inside + (i * returner_white);
    Serial.print("   ");
    Serial.print(returner_white);
    Serial.print("   ");
    Serial.println(white_color_data);*/
 // }
  //timer2 = millis();}

  
}

void control_brightness(){
  if(millis() - timer3 >= sampletime){
    FastLED.setBrightness(brightness);
  val = analogRead(A0);
  speed = (abs(val - 661.5) * kp + abs(val - 661.5) * kd / sampletime );

  if(val > 800) brightness -= speed;
  else if(val < 800) brightness += speed;
  if(brightness >= 255) brightness = 255;
  if(brightness <= 50) brightness = 50;

  timer3 = millis();
}
}

void color_control_second_resetor(){
  returner = 1;
  control_second_color = 0;
}

void control_second_color_interrupt(){
   byte color_second_B_inside;
   control_second_color ++;
   if(control_second_color >= 255) {
     returner *= -1;
     control_second_color = 0;
   }
   if(control_second_color == 255 && returner == -1) color_second_B_inside = 255;
   if(color_second_B >= 10) color_second_B = color_second_B_inside + control_second_color * returner;
   else color_second_B = 10;

   //control_white_timer_inside ++;

}

void control_led(){


  if(millis() - timer2 >= 6.1803){
   //if(control_second_color >= 255) {

  
      byte color_white_inside;
 //for(byte control_second_color = 0 ; control_second_color <= 100 ; control_second_color++){
    if(control_white_color < 155) control_white_color++;
    else control_white_color = 0;
    if(control_white_color == 0) returner_white *= -1;
    if(returner_white == -1) color_white_inside = 255;
    if(returner_white == 1) color_white_inside = 100;
    white_color_data = color_white_inside + (control_white_color * returner_white);
        Serial.print("   ");
    Serial.print(returner_white);
    Serial.print("   ");
    Serial.println(white_color_data);
 // }
      control_white_timer_inside = 0;
      timer2 = millis();
   }
   


 

  for(byte white_show = 0 ; white_show <= 55 ; white_show +=5){
    leds[white_show] = CRGB(white_color_data, white_color_data, white_color_data);
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
    leds[second_ - 1] = CRGB(white_color_data, white_color_data, white_color_data);
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
    leds[minute_ - 1] = CRGB(white_color_data, white_color_data, white_color_data);
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
    leds[inside_hour - 1] = CRGB(white_color_data, white_color_data, white_color_data);
   }
   else{leds[inside_hour - 1] = CRGB(0, 0, 0);}
}
}
}
}
}
    

    //hour led control END

    //cover led control BEGIN
    if(color_second_B == 255){
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
  }
  else{
    if(second_ == minute_){
      leds[second_] = CRGB(color_second_R, color_minute_G, color_second_B);
    }
    if(second_ == inside_hour){
      leds[second_] = CRGB(color_hour_R, color_second_G, color_second_B);
    }
    if(minute_ == inside_hour){
      leds[minute_] = CRGB(color_hour_R, color_minute_G, color_minute_B);
    }
    if(second_ == minute_ && second_ == hour_){
        leds[second_] = CRGB(color_hour_R, color_minute_G, color_second_B);
    }
    for(byte i = 0 ; i <= 55 ; i +=5){
      if(second_ == i){
        if(second_ == minute_) leds[second_] = CRGB(color_second_R, color_minute_G, color_second_B);
        else if(second_ == inside_hour) leds[second_] = CRGB(color_hour_R, color_second_G, color_second_B);
        else if(second_ == minute_ && second_ == inside_hour) leds[second_] = CRGB(color_hour_R, color_minute_G, color_second_B);
        else leds[second_] = CRGB(white_color_data, white_color_data, color_second_B);
      }
    }
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



