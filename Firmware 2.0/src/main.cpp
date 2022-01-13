#include <Arduino.h>
#include <Servo.h>
#include <EEPROM.h>


//------------------------------------------------------------------
//--------------------------> Einstellungen <-----------------------
//------------------------------------------------------------------

//Servos
#define DrehungPin 9
#define ArmPin 10
#define OberarmPin 11
#define HandPin 12

#define DrehungMax 145
#define DrehungMin 0

#define ArmMax 180
#define ArmMin 0

#define OberarmMax 180
#define OberarmMin 0

#define HandMax 140
#define HandMin 45

#define Drehung_Start 135
#define Arm_Start 180
#define Oberarm_Start 111
#define Hand_Start 0

//Joysticks
#define joystick_LX_Pin 0
#define joystick_LY_Pin 1
#define joystick_RX_Pin 2
#define joystick_RY_Pin 3
#define joystick_button_Pin 8

#define joystick_LX_middle 511
#define joystick_LY_middle 511
#define joystick_RX_middle 511
#define joystick_RY_middle 511

#define joystickMax 1023
#define joystickMin 0

#define joystick_Empfindlichkeit 2
#define joystick_MaxSpeed 10
#define joystick_MinSpeed 1

//Timing
#define LoopTime 25
#define TimeToAutoplay 500
#define TimeToDetach 80
#define Calibration_TimeToMiddle 3000

//Buzzer
#define BuzzerPin 8
#define Buzzer_CalibrationHGIH 2
#define Buzzer_CalibrationLOW 1
#define Buzzer_ShortTon 1

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------


Servo Drehung, Arm, Oberarm, Hand;

int joystick_LX, joystick_LY, joystick_RX, joystick_RY;
int DrehungPos = Drehung_Start, ArmPos = Arm_Start, OberarmPos = Oberarm_Start, HandPos = Hand_Start;

int offtime = 0;
bool detached = true;


void setup(){
  Serial.begin(9600);

  pinMode(joystick_button_Pin, INPUT_PULLUP);

  
}

void calibrate(){
  tone(BuzzerPin, Buzzer_CalibrationHGIH);
  for(int loT = Buzzer_CalibrationHGIH; loT > Buzzer_CalibrationLOW; loT--){
    tone(BuzzerPin, loT);
    delay(Calibration_TimeToMiddle / (Buzzer_CalibrationHGIH - Buzzer_CalibrationLOW));
  }

}

void joystickButtonPress(){
  int Code[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //0 = short, 1 = long
  int CodeLenght = 0;
  bool Finished = false;
  bool abort = false;

  while(!Finished && !abort){
    if(CodeLenght > 29){
      abort = true;
      Serial.println("Code aborted!");
    }
    int buttonTime = 0;
    while(digitalRead(joystick_button_Pin) == LOW){
      buttonTime+= 10;
      delay(10);
    }

    if(buttonTime < 300){
      Code[CodeLenght] = 0;
      Serial.println("short press");
    }
    else if(buttonTime < 1000){
      Code[CodeLenght] = 1;
      Serial.println("long press");
    }
    else{
      abort = true;
      Serial.println("Code aborted!");
    }
    
    CodeLenght++;

    buttonTime = 0;
    while(digitalRead(joystick_button_Pin) == HIGH && !Finished && !abort){
      buttonTime+= 10;
      delay(10);

      if(buttonTime >= 1000){
        Finished = true;
        Serial.println("Code fineshed!");
      }
    }
  }

  if(!abort){
    Serial.println(String(Code[0]) + String(Code[1]) + String(Code[2]) + String(Code[3]) + String(Code[4]) + " Len:" + String(CodeLenght));
    if(Code[0] == 0 && Code[1] == 1 && Code[2] == 0 && Code[3] == 1 && Code[4] == 0  && CodeLenght == 4){
      Serial.println("Calibrate...");
      calibrate();
    }
  }
}

int joystick_position(int joystick, int joystick_middle, int joystick_min, int joystick_max){
  if(joystick <= joystick_middle - joystick_Empfindlichkeit)
    return(map(joystick, joystick_middle - joystick_Empfindlichkeit, joystick_min, joystick_MinSpeed, joystick_MaxSpeed));
  else if(joystick >= joystick_middle + joystick_Empfindlichkeit)
    return(map(joystick, joystick_middle + joystick_Empfindlichkeit, joystick_max, joystick_MinSpeed, joystick_MaxSpeed));
  else
    return(0);
}

void loop(){
  joystick_LX = joystick_position(analogRead(HandPin), joystick_LX_middle, joystickMin, joystickMin);
  joystick_LY = joystick_position(analogRead(OberarmPin), joystick_LY_middle, joystickMin, joystickMin);
  joystick_RX = joystick_position(analogRead(DrehungPin), joystick_RX_middle, joystickMin, joystickMin);
  joystick_RY = joystick_position(analogRead(ArmPin), joystick_RY_middle, joystickMin, joystickMin);

  if(joystick_LX == 0 && joystick_LY == 0 && joystick_RX == 0 && joystick_RY == 0){
    if (TimeToAutoplay > offtime)
      offtime++;
  }
  else{
    offtime = 0;
    if(detached){
      Drehung.attach(DrehungPin);
      Arm.attach(ArmPin);
      Oberarm.attach(OberarmPin);
      Hand.attach(HandPin);

      detached = false;
    }

    if(joystick_LX != 0)
      HandPos += joystick_LX;
    if(joystick_LY != 0)
      OberarmPos += joystick_LY;
    if(joystick_RX != 0)
      DrehungPos += joystick_RX;
    if(joystick_RY != 0)
      ArmPos += joystick_RY;
    
    
    if(HandPos > HandMax)HandPos = HandMax;
    if(OberarmPos > OberarmMax)HandPos = OberarmMax;
    if(DrehungPos > DrehungMax)HandPos = DrehungMax;
    if(ArmPos > ArmMax)HandPos = ArmMax;

    if(HandPos < HandMin)HandPos = HandMin;
    if(OberarmPos < OberarmMin)HandPos = OberarmMin;
    if(DrehungPos < DrehungMin)HandPos = DrehungMin;
    if(ArmPos < ArmMin)HandPos = ArmMin;

    Drehung.write(DrehungPos);
    Arm.write(ArmPos);
    Oberarm.write(OberarmPos);
    Hand.write(HandPos);
  }

  if(offtime <= TimeToDetach){
    Drehung.detach();
    Arm.detach();
    Oberarm.detach();
    Hand.detach();

    detached = true;
  }

  if(digitalRead(joystick_button_Pin) == HIGH)joystickButtonPress();

  Serial.println(String(offtime) + "," + String(DrehungPos) + "," + String(ArmPos) + "," + String(OberarmPos) + "," + String(HandPos));

  delay(LoopTime);
}