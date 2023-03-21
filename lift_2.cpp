#include <Arduino.h>
#include<Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#define BUTTON_PIN A0

#define sda A4 
#define scl A5

#define echo 6
#define trig 7

#define IR_PIN1  8
#define IR_PIN2  9
#define IR_PIN3  10
#define IR_PIN4  11

#define MOT_PIN1  3
#define MOT_PIN2  4

float tijd;
int dist_tmp= 0;

int cur_pos;


int IRsensorRead(int pos)
{
 int return_val= 0;
 
 if(pos == 1)
 {
  if(digitalRead(IR_PIN1) == 0)
   return_val= 1;
 }

 if(pos == 2)
 {
  if(digitalRead(IR_PIN2) == 0)
   return_val= 2;
 }

 if(pos == 3)
 {
  if(digitalRead(IR_PIN3) == 0)
   return_val= 3;
 }

 if(pos == 4)
 {
  if(digitalRead(IR_PIN4) == 0)
   return_val= 4;
 }

 return return_val;
}


void motor_UP()
{
 digitalWrite(MOT_PIN1,HIGH);
 digitalWrite(MOT_PIN2,LOW);
}

void motor_DOWN()
{
 digitalWrite(MOT_PIN1,LOW);
 digitalWrite(MOT_PIN2,HIGH);
}

void motor_STOP()
{
 digitalWrite(MOT_PIN1,LOW);
 digitalWrite(MOT_PIN2,LOW);
}


void setup()
{
pinMode(echo, INPUT);pinMode(trig, OUTPUT);
  // I2c LCD pinen
pinMode(sda,OUTPUT);
pinMode(scl,OUTPUT);
pinMode(BUTTON_PIN,INPUT); 

 
 pinMode(IR_PIN1,INPUT);
 pinMode(IR_PIN2,INPUT);
 pinMode(IR_PIN3,INPUT);
 pinMode(IR_PIN4,INPUT);

 pinMode(MOT_PIN1,OUTPUT);
 pinMode(MOT_PIN2,OUTPUT);
 // lcd initialize
lcd.init();                    
lcd.backlight();
lcd.print("SETUP");


 //Move down to first position
 while(IRsensorRead(1) != 1)
   motor_DOWN();

 motor_STOP();
 cur_pos= 1;
 // show first position on screen
 lcd.clear();
 lcd.setCursor(0,1);
 lcd.print("Position : ");
 lcd.print(cur_pos);
 }

float MeasureDistance()
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(2);
  digitalWrite(trig, LOW);
  tijd = pulseIn(echo, HIGH);
  float dist = tijd / 58.2;  
  return dist;

}

int readAnalogButton()
{
 int button= analogRead(BUTTON_PIN);
 if(button > 1000) return 0; //default value. no buttons pushed 1024
 if(button == 0) return 1;
 if(button < 600) return 2;
 if(button < 700) return 3;
 if(button < 800) return 4;

}



void move_to_position(int pos)
{
 while(cur_pos != pos)
 {
  if(pos > cur_pos )
   motor_UP();
  else
   motor_DOWN();

  if(IRsensorRead(pos) == pos)
  {
    cur_pos= pos;
    lcd.setCursor(0,1);
    lcd.print("position : ");
    lcd.print(cur_pos);
  }
 }
 motor_STOP();

}

void loop()
{
 int but= readAnalogButton();
 if(but != 0) 
    move_to_position(but);
 
 float dist = MeasureDistance();
 if (dist!=dist_tmp)
  {
    lcd.setCursor(0,0);
    lcd.print("Afstand = ");
    lcd.print(dist);
    lcd.print("cm");
    dist_tmp= dist;
    delay(100);
   
  }
 
}
