#include <LiquidCrystal_I2C.h>
#include <Wire.h> 

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define BUTTON_PIN A0

#define IR_PIN1  8
#define IR_PIN2  9
#define IR_PIN3  10
#define IR_PIN4  11

#define MOT_PIN1  3
#define MOT_PIN2  4

int cur_pos;

void setup()
{
 Serial.begin(9600);  
 pinMode(BUTTON_PIN,INPUT); 
 
 pinMode(IR_PIN1,INPUT);
 pinMode(IR_PIN2,INPUT);
 pinMode(IR_PIN3,INPUT);
 pinMode(IR_PIN4,INPUT);

 pinMode(MOT_PIN1,OUTPUT);
 pinMode(MOT_PIN2,OUTPUT);

 lcd.init();
 lcd.clear();
 
 //Move down to first position
 while(IRsensorRead(1) != 1)
  motor_DOWN();

 motor_STOP();
 cur_pos= 1;
}

int readAnalogButton()
{
 int button= analogRead(BUTTON_PIN);
 if(button > 1000) return 0; //default value. no buttons pushed
 if(button == 0) return 1;
 if(button < 600) return 2;
 if(button < 700) return 3;
 if(button < 800) return 4;
}

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

void move_to_position(int pos)
{
 while(cur_pos != pos)
 {
  if(pos > cur_pos )
   motor_UP();
  else
   motor_DOWN();

  if(IRsensorRead(pos) == pos)
    cur_pos= pos;
 }
 motor_STOP();
}

void loop()
{
 int but= readAnalogButton();
 if(but != 0) 
 {
  move_to_position(but);
  lcd.setCursor(4,1);
  lcd.print("position:");
  lcd.setCursor(4,2);
  lcd.print(but);
  //Serial.println(but);
 }
}
