#include <Arduino.h>
#include <Wire.h>    // het komt met LCD display bij
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

//________________________________________________

// Lcd display pinnen
#define sda A4   // We have specified the 'analog pins' by putting the letter 'A' at the beginning.
#define scl A5

// Ultrasonic sensor pinnen   
#define echo 6
#define trig 7
// IR sensoren
int IRsensor1= 8, IRsensor2 = 9,IRsensor3 = 10,IRsensor4 = 11 ;
//MOTOR pinnen
int SpeedPin= A3;// to adjust motor speed
int IN1 = 13;  // play right  
int IN2= 12;  // play left
// Buttonen pinnen
const int buttonPin1 = 2;
const int buttonPin2 = 3;
const int buttonPin3 = 4;
const int buttonPin4= 5;
//-------------------------------------------------

 // de huidige lezing van de input pin
 int buttonState1;
 int buttonState2; 
 int buttonState3; 
 int buttonState4; 

// de vorige lezing van de invoerpin
 int lastButtonState1=LOW; 
 int lastButtonState2=LOW;
 int lastButtonState3=LOW;
 int lastButtonState4=LOW;
 // for ultrasonic sensor 
 float tijd;
 int cmt_tmp= 0;
 unsigned long lastDebounceTime = 0;  // de laatste keer dat de uitvoerpin werd omgeschakeld
 unsigned long debounceDelay = 50;     // de debounce-tijd; verhogen als de outputtrillen
 // Motor variables
  int m1;
  int m2;
  boolean motorState;

  // Auto variables
  int doelAutoPlaats[4]={1,2,3,4};
  int lastAutoPlaats;

// IRsensor variables
  int IRsensor[4] = {IRsensor1,IRsensor2,IRsensor3,IRsensor4};   // bunu bir soralim bu sekilde calisir mi?


//----------------------------------------------------


void setup()
 {
  pinMode(buttonPin1,INPUT);pinMode(buttonPin2,INPUT);pinMode(buttonPin3,INPUT);pinMode(buttonPin4,INPUT);
  // voor mototbesturing pinnen
  pinMode(IN1,OUTPUT);pinMode(IN2,OUTPUT);pinMode(SpeedPin,OUTPUT);
  // IR sensor en Ultrasonic sensor
  pinMode(IRsensor1,INPUT);pinMode(IRsensor2,INPUT);pinMode(IRsensor3,INPUT);pinMode(IRsensor4,INPUT);
  pinMode(echo, INPUT);pinMode(trig, OUTPUT);
  
  // I2c LCD pinen
  pinMode(sda,OUTPUT);
  pinMode(scl,OUTPUT);
  
  // initialize the lcd 
  lcd.init();                    
  lcd.backlight();
  lcd.print("Hallo");
  delay(1000);
  
// Serial.begin(9600);

}

//The function that determines the position of your vehicle to us with IR sensors.
int LastAutoPlaats()
{

  for (int i = 0; i < 4; i++) 
  {
    int valIR = digitalRead(IRsensor[i]);
    
    if (valIR == HIGH) 
    {
      lastAutoPlaats= i;
      
    }
    
  }

  return lastAutoPlaats;
 

}


//functions for motor
void MeasureDistance()
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(2);
  digitalWrite(trig, LOW);
  tijd = pulseIn(echo, HIGH);
  float cmt = tijd / 58.2;         // We vinden ervan de afstand.
  if(cmt != cmt_tmp)
  {
   delay(500);
   lcd.setCursor(0,0);
   lcd.print("Afstand =");
   lcd.print(cmt);
   lcd.print("cm");
   cmt_tmp= cmt;
  }
  
}

void _motorRight(){
  digitalWrite(SpeedPin,40);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
}

void _motorLeft(){
  digitalWrite(SpeedPin,40);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
}

void  _motorStop()
{
   
   digitalWrite(IN1,HIGH);
   digitalWrite(IN2,HIGH);
 }



//It checks if the motor is running and sends us a boolean value.
int _mototState()
{
  int m1= digitalRead(IN1);
  int m2= digitalRead(IN2);
  if (m1==HIGH && m2 ==HIGH){
    return false; // motor is bloked
  }else {
     return true;  // motor working
  }
  
 }




//functions for button
void Button4()
{
 
 int reading4 = digitalRead(buttonPin4);
 if (reading4 != lastButtonState4) 
 {
    // reset the debouncing timer
    lastDebounceTime = millis();
 }

 if ((millis() - lastDebounceTime) > debounceDelay) 
 {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading4 != buttonState4) 
    {
      buttonState4 = reading4;
      motorState = _mototState();

      //Als de buton 4 wordt ingedrukt en de motor niet start
      if (buttonState4 == HIGH && motorState==false)    // mototState = false ----> the motor is blocked.
      {
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Buton 4 = Aan");
        
        // read actuel plaats van auto
        lastAutoPlaats= LastAutoPlaats();

        if (lastAutoPlaats < doelAutoPlaats[3])
        {
          while (lastAutoPlaats < doelAutoPlaats[3] )  // int doelAutoPlaats[4]={1,2,3,4};
          {
           _motorRight();
           lastAutoPlaats= LastAutoPlaats();

          }
          // If we have reached the target point, stop the motor.
        }else
        {
          _motorStop();
          lastAutoPlaats= LastAutoPlaats();
        }
         

        
      }
    }
  }


lastButtonState4 = reading4;
}

void Button3(){
 int reading3 = digitalRead(buttonPin3);

 if (reading3 != lastButtonState3) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
   
    if (reading3 != buttonState3) {
      buttonState3 = reading3;
      motorState = _mototState();

      // Als de button 3 wordt ingedrukt en de motor niet start
      if (buttonState3 == HIGH && motorState==false) // mototState = false ----> the motor is blocked.
      {
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Buton 3 = Aan");
        // read actuel plaats van auto
        lastAutoPlaats= LastAutoPlaats();

        if (lastAutoPlaats < doelAutoPlaats[2]) //  doelAutoPlaats[4]={1,2,3,4}; ---> 3
        {
          while (lastAutoPlaats < doelAutoPlaats[2] )
          {
           _motorRight();
           lastAutoPlaats= LastAutoPlaats();

          }
          // If we have reached the target point, stop the engine.
        }else if(lastAutoPlaats > doelAutoPlaats[2])
        {
          while (lastAutoPlaats > doelAutoPlaats[2] )
          {
           _motorLeft();
           lastAutoPlaats= LastAutoPlaats();

          }
          
        }else
        {
          _motorStop();
        }
      }
    }
  }
lastButtonState3 = reading3;
}


void Button2()
{
 int reading2 = digitalRead(buttonPin2);
 if (reading2 != lastButtonState2) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading2 != buttonState2) {
      buttonState2 = reading2;

    //Als de buton 2 wordt ingedrukt en de motor niet start
      if (buttonState2 == HIGH && motorState==false) //mototState = false ----> the motor is blocked.
      {
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Buton 2 = Aan");
        
        // read actuel plaats van auto
        lastAutoPlaats= LastAutoPlaats();

        if (lastAutoPlaats < doelAutoPlaats[1])
        {
          while (lastAutoPlaats < doelAutoPlaats[1] )
          {
           _motorRight();
           lastAutoPlaats= LastAutoPlaats();

          }
          // If we have reached the target point, stop the engine.
        }else if(lastAutoPlaats > doelAutoPlaats[1])
        {
          while (lastAutoPlaats > doelAutoPlaats[1] )
          {
           _motorLeft();
           lastAutoPlaats= LastAutoPlaats();

          }
          
        }else{
          _motorStop();
        }
      }
    }
  }
lastButtonState2 = reading2;
}


void Button1()
{
 int reading1 = digitalRead(buttonPin1);

 if (reading1 != lastButtonState1) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
   
    if (reading1 != buttonState1) {
      buttonState1 = reading1;

      //Als de buton 1 wordt ingedrukt en de motor niet start
      if (buttonState1 == HIGH && motorState==false)  //mototState = false ----> the motor is blocked.
      {
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Buton 1 = Aan");
        // read actuel plaats van auto
        lastAutoPlaats= LastAutoPlaats();

        if (lastAutoPlaats > doelAutoPlaats[0])
        {
          while (lastAutoPlaats > doelAutoPlaats[0] )
          {
           _motorLeft();
           lastAutoPlaats= LastAutoPlaats();

          }
          // If we have reached the target point, stop the motor.
        }else 
        {
          _motorStop();
          lastAutoPlaats= LastAutoPlaats();
        }
          
       

      }
    }
  }
lastButtonState1 = reading1;
}


void loop() 
{
 MeasureDistance();
 
 Button1();
 
 MeasureDistance();
 
 Button2();
 
 MeasureDistance();
 
 Button3();
 
 MeasureDistance();
 
 Button4();
 MeasureDistance();

}

Not: debounce ile ilgili lbir problemim var ve calismadi. bunu gozden gecirmeliyim.