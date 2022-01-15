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

#define HandMax 130
#define HandMin 45

#define Drehung_Start 135
#define Arm_Start 180
#define Oberarm_Start 111
#define Hand_Start 130

//Joysticks
#define joystick_LX_Pin 0
#define joystick_LY_Pin 1
#define joystick_RX_Pin 2
#define joystick_RY_Pin 3
#define joystick_button_Pin 8

#define joystick_Empfindlichkeit 4
#define joystick_MaxSpeed 5
#define joystick_MinSpeed 1

//Timing
#define LoopTime 40
#define TimeToAutoplay 1000
#define TimeToDetach 80
#define Calibration_TimeToMiddle 3000
#define Calibration_messureTime 5

//Buzzer
#define BuzzerPin 7
#define Buzzer_CalibrationHGIH 2000
#define Buzzer_CalibrationLOW 100
#define Buzzer_ShortTon 1000
#define Buzzer_ShortTone_Time 50

//EEPROM
#define EEPROM_Code 6182 //index 0
#define EEPROM_joystick_LX_middle 20
#define EEPROM_joystick_LY_middle 40
#define EEPROM_joystick_RX_middle 60
#define EEPROM_joystick_RY_middle 80
#define EEPROM_joystick_LX_max 100
#define EEPROM_joystick_LY_max 120
#define EEPROM_joystick_RX_max 140
#define EEPROM_joystick_RY_max 160
#define EEPROM_joystick_LX_min 180
#define EEPROM_joystick_LY_min 200
#define EEPROM_joystick_RX_min 220
#define EEPROM_joystick_RY_min 240

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------


Servo Drehung, Arm, Oberarm, Hand;

int joystick_LX, joystick_LY, joystick_RX, joystick_RY;
int joystick_LX_middle, joystick_LY_middle, joystick_RX_middle, joystick_RY_middle;
int joystick_LX_max, joystick_LY_max, joystick_RX_max, joystick_RY_max;
int joystick_LX_min, joystick_LY_min, joystick_RX_min, joystick_RY_min;
int DrehungPos = Drehung_Start, ArmPos = Arm_Start, OberarmPos = Oberarm_Start, HandPos = Hand_Start;

int offtime = 0;
bool detached = true;
int eeprom = 0;


void calibrate(){
  tone(BuzzerPin, Buzzer_CalibrationHGIH);
  for(int loT = Buzzer_CalibrationHGIH; loT > Buzzer_CalibrationLOW; loT--){
    tone(BuzzerPin, loT);
    delay(Calibration_TimeToMiddle / (Buzzer_CalibrationHGIH - Buzzer_CalibrationLOW));
  }

  Serial.print("Get Middle Positions: ");

  int joystick_LX_middle_r1 = analogRead(joystick_LX_Pin);
  delay(Calibration_messureTime);
  int joystick_LX_middle_r2 = analogRead(joystick_LX_Pin);
  delay(Calibration_messureTime);
  int joystick_LX_middle_r3 = analogRead(joystick_LX_Pin);
  delay(Calibration_messureTime);
  int joystick_LX_middle_r4 = analogRead(joystick_LX_Pin);
  delay(Calibration_messureTime);
  int joystick_LX_middle_r5 = analogRead(joystick_LX_Pin);
  int joystick_LX_middle_av = (joystick_LX_middle_r1 + joystick_LX_middle_r2 + joystick_LX_middle_r3 + joystick_LX_middle_r4 + joystick_LX_middle_r5) / 5;

  Serial.print("LX:" + String(joystick_LX_middle_av));

  int joystick_LY_middle_r1 = analogRead(joystick_LY_Pin);
  delay(Calibration_messureTime);
  int joystick_LY_middle_r2 = analogRead(joystick_LY_Pin);
  delay(Calibration_messureTime);
  int joystick_LY_middle_r3 = analogRead(joystick_LY_Pin);
  delay(Calibration_messureTime);
  int joystick_LY_middle_r4 = analogRead(joystick_LY_Pin);
  delay(Calibration_messureTime);
  int joystick_LY_middle_r5 = analogRead(joystick_LY_Pin);
  int joystick_LY_middle_av = (joystick_LY_middle_r1 + joystick_LY_middle_r2 + joystick_LY_middle_r3 + joystick_LY_middle_r4 + joystick_LY_middle_r5) / 5;

  Serial.print(" LY:" + String(joystick_LY_middle_av));

  int joystick_RX_middle_r1 = analogRead(joystick_RX_Pin);
  delay(Calibration_messureTime);
  int joystick_RX_middle_r2 = analogRead(joystick_RX_Pin);
  delay(Calibration_messureTime);
  int joystick_RX_middle_r3 = analogRead(joystick_RX_Pin);
  delay(Calibration_messureTime);
  int joystick_RX_middle_r4 = analogRead(joystick_RX_Pin);
  delay(Calibration_messureTime);
  int joystick_RX_middle_r5 = analogRead(joystick_RX_Pin);
  int joystick_RX_middle_av = (joystick_RX_middle_r1 + joystick_RX_middle_r2 + joystick_RX_middle_r3 + joystick_RX_middle_r4 + joystick_RX_middle_r5) / 5;

  Serial.print(" RX:" + String(joystick_RX_middle_av));

  int joystick_RY_middle_r1 = analogRead(joystick_RY_Pin);
  delay(Calibration_messureTime);
  int joystick_RY_middle_r2 = analogRead(joystick_RY_Pin);
  delay(Calibration_messureTime);
  int joystick_RY_middle_r3 = analogRead(joystick_RY_Pin);
  delay(Calibration_messureTime);
  int joystick_RY_middle_r4 = analogRead(joystick_RY_Pin);
  delay(Calibration_messureTime);
  int joystick_RY_middle_r5 = analogRead(joystick_RY_Pin);
  int joystick_RY_middle_av = (joystick_RY_middle_r1 + joystick_RY_middle_r2 + joystick_RY_middle_r3 + joystick_RY_middle_r4 + joystick_RY_middle_r5) / 5;

  Serial.println(" RY:" + String(joystick_RY_middle_av));

  EEPROM.put(EEPROM_joystick_LX_middle, joystick_LX_middle_av);
  EEPROM.put(EEPROM_joystick_LY_middle, joystick_LY_middle_av);
  EEPROM.put(EEPROM_joystick_RX_middle, joystick_RX_middle_av);
  EEPROM.put(EEPROM_joystick_RY_middle, joystick_RY_middle_av);
  joystick_LX_middle = joystick_LX_middle_av;
  joystick_LY_middle = joystick_LY_middle_av;
  joystick_RX_middle = joystick_RX_middle_av;
  joystick_RY_middle = joystick_RY_middle_av;

  Serial.println("Saved");
  
  tone(BuzzerPin, Buzzer_ShortTon);
  delay(Buzzer_ShortTone_Time);
  noTone(BuzzerPin);

  Serial.println("Wait for Input...");

  joystick_LX_max = joystick_LX_middle_av;
  joystick_LY_max = joystick_LY_middle_av;
  joystick_RX_max = joystick_RX_middle_av;
  joystick_RY_max = joystick_RY_middle_av;
  joystick_LX_min = joystick_LX_middle_av;
  joystick_LY_min = joystick_LY_middle_av;
  joystick_RX_min = joystick_RX_middle_av;
  joystick_RY_min = joystick_RY_middle_av;

  while(digitalRead(joystick_button_Pin) == HIGH){
    int joystick_LX_rout = analogRead(joystick_LX_Pin);
    int joystick_LY_rout = analogRead(joystick_LY_Pin);
    int joystick_RX_rout = analogRead(joystick_RX_Pin);
    int joystick_RY_rout = analogRead(joystick_RY_Pin);

    if(joystick_LX_rout > joystick_LX_max)joystick_LX_max = joystick_LX_rout;
    if(joystick_LY_rout > joystick_LY_max)joystick_LY_max = joystick_LY_rout;
    if(joystick_RX_rout > joystick_RX_max)joystick_RX_max = joystick_RX_rout;
    if(joystick_RY_rout > joystick_RY_max)joystick_RY_max = joystick_RY_rout;
    if(joystick_LX_rout < joystick_LX_min)joystick_LX_min = joystick_LX_rout;
    if(joystick_LY_rout < joystick_LY_min)joystick_LY_min = joystick_LY_rout;
    if(joystick_RX_rout < joystick_RX_min)joystick_RX_min = joystick_RX_rout;
    if(joystick_RY_rout < joystick_RY_min)joystick_RY_min = joystick_RY_rout;

    delay(1);
  }

  Serial.println("New Max: LX:" + String(joystick_LX_max) + " LY:" + String(joystick_LY_max) + " RX:" + String(joystick_RX_max) + " RY:" + String(joystick_RY_max));
  Serial.println("New Min: LX:" + String(joystick_LX_min) + " LY:" + String(joystick_LY_min) + " RX:" + String(joystick_RX_min) + " RY:" + String(joystick_RY_min));

  EEPROM.put(EEPROM_joystick_LX_max, joystick_LX_max);
  EEPROM.put(EEPROM_joystick_LY_max, joystick_LY_max);
  EEPROM.put(EEPROM_joystick_RX_max, joystick_RX_max);
  EEPROM.put(EEPROM_joystick_RY_max, joystick_RY_max);
  EEPROM.put(EEPROM_joystick_LX_min, joystick_LX_min);
  EEPROM.put(EEPROM_joystick_LY_min, joystick_LY_min);
  EEPROM.put(EEPROM_joystick_RX_min, joystick_RX_min);
  EEPROM.put(EEPROM_joystick_RY_min, joystick_RY_min);

  Serial.println("Saved");

  tone(BuzzerPin, Buzzer_ShortTon);
  delay(Buzzer_ShortTone_Time);
  noTone(BuzzerPin);
  delay(25);
  tone(BuzzerPin, Buzzer_ShortTon);
  delay(Buzzer_ShortTone_Time);
  noTone(BuzzerPin);

  Serial.println("Calibration finished!");
  Serial.println(eeprom);
  delay(2000);
}

void setup(){
  Serial.begin(9600);

  pinMode(joystick_button_Pin, INPUT_PULLUP);

  EEPROM.get(1, eeprom);
  if(eeprom != EEPROM_Code){
    for (unsigned int i = 1 ; i < EEPROM.length() ; i++) {
      EEPROM.put(i, 0);
    }

    EEPROM.put(1, EEPROM_Code);
    calibrate();
  }
  else{
    EEPROM.get(EEPROM_joystick_LX_middle, joystick_LX_middle);
    EEPROM.get(EEPROM_joystick_LY_middle, joystick_LY_middle);
    EEPROM.get(EEPROM_joystick_RX_middle, joystick_RX_middle);
    EEPROM.get(EEPROM_joystick_RY_middle, joystick_RY_middle);

    EEPROM.get(EEPROM_joystick_LX_max, joystick_LX_max);
    EEPROM.get(EEPROM_joystick_LY_max, joystick_LY_max);
    EEPROM.get(EEPROM_joystick_RX_max, joystick_RX_max);
    EEPROM.get(EEPROM_joystick_RY_max, joystick_RY_max);
    EEPROM.get(EEPROM_joystick_LX_min, joystick_LX_min);
    EEPROM.get(EEPROM_joystick_LY_min, joystick_LY_min);
    EEPROM.get(EEPROM_joystick_RX_min, joystick_RX_min);
    EEPROM.get(EEPROM_joystick_RY_min, joystick_RY_min);
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
    return(-map(joystick, joystick_middle - joystick_Empfindlichkeit, joystick_min, joystick_MinSpeed, joystick_MaxSpeed));
  else if(joystick >= joystick_middle + joystick_Empfindlichkeit)
    return(map(joystick, joystick_middle + joystick_Empfindlichkeit, joystick_max, joystick_MinSpeed, joystick_MaxSpeed));
  else
    return(0);
}

void loop(){
  joystick_LX = joystick_position(analogRead(joystick_LX_Pin), joystick_LX_middle, joystick_LX_min, joystick_LX_max);
  joystick_LY = joystick_position(analogRead(joystick_LY_Pin), joystick_LY_middle, joystick_LY_min, joystick_LY_max);
  joystick_RX = joystick_position(analogRead(joystick_RX_Pin), joystick_RX_middle, joystick_RX_min, joystick_RX_max);
  joystick_RY = joystick_position(analogRead(joystick_RY_Pin), joystick_RY_middle, joystick_RY_min, joystick_RY_max);

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
    if(OberarmPos > OberarmMax)OberarmPos = OberarmMax;
    if(DrehungPos > DrehungMax)DrehungPos = DrehungMax;
    if(ArmPos > ArmMax)ArmPos = ArmMax;

    if(HandPos < HandMin)HandPos = HandMin;
    if(OberarmPos < OberarmMin)OberarmPos = OberarmMin;
    if(DrehungPos < DrehungMin)DrehungPos = DrehungMin;
    if(ArmPos < ArmMin)ArmPos = ArmMin;

    Drehung.write(DrehungPos);
    Arm.write(ArmPos);
    Oberarm.write(OberarmPos);
    Hand.write(HandPos);
  }

  if(offtime >= TimeToDetach){
    Drehung.detach();
    Arm.detach();
    Oberarm.detach();
    Hand.detach();

    detached = true;
  }

  if(digitalRead(joystick_button_Pin) == LOW){
    Drehung.detach();
    Arm.detach();
    Oberarm.detach();
    Hand.detach();

    detached = true;

    joystickButtonPress();
  }

  Serial.println(String(offtime) + "," + String(DrehungPos) + "," + String(ArmPos) + "," + String(OberarmPos) + "," + String(HandPos));

  delay(LoopTime);
}