#include <Arduino.h>
#include <Servo.h>
#include <EEPROM.h>
#include "config.h"

Servo Drehung, Arm, Oberarm, Hand;

int joystick_LX, joystick_LY, joystick_RX, joystick_RY;
int joystick_LX_middle, joystick_LY_middle, joystick_RX_middle, joystick_RY_middle;
int joystick_LX_max, joystick_LY_max, joystick_RX_max, joystick_RY_max;
int joystick_LX_min, joystick_LY_min, joystick_RX_min, joystick_RY_min;
int DrehungPos = Drehung_Start, ArmPos = Arm_Start, OberarmPos = Oberarm_Start, HandPos = Hand_Start;

int offtime = 0;
bool detached = true;
int eeprom = 0;

void calibrateMiddle(){
  Serial.print("Get Middle Positions: ");

  joystick_LX_middle = 0;
  for(int i = Calibration_messureCount; i > 0; i--){
    joystick_LX_middle += analogRead(joystick_LX_Pin);
    delay(Calibration_messureTime);
  }
  joystick_LX_middle = joystick_LX_middle / Calibration_messureCount;

  Serial.print("LX:" + String(joystick_LX_middle));

  joystick_LY_middle = 0;
  for(int i = Calibration_messureCount; i > 0; i--){
    joystick_LY_middle += analogRead(joystick_LY_Pin);
    delay(Calibration_messureTime);
  }
  joystick_LY_middle = joystick_LY_middle / Calibration_messureCount;

  Serial.print(" LY:" + String(joystick_LY_middle));

  joystick_RX_middle = 0;
  for(int i = Calibration_messureCount; i > 0; i--){
    joystick_RX_middle += analogRead(joystick_RX_Pin);
    delay(Calibration_messureTime);
  }
  joystick_RX_middle = joystick_RX_middle / Calibration_messureCount;

  Serial.print(" RX:" + String(joystick_RX_middle));

  joystick_RY_middle = 0;
  for(int i = Calibration_messureCount; i > 0; i--){
    joystick_RY_middle += analogRead(joystick_RY_Pin);
    delay(Calibration_messureTime);
  }
  joystick_RY_middle = joystick_RY_middle / Calibration_messureCount;

  Serial.println(" RY:" + String(joystick_RY_middle));

  Serial.println("Saved");
}

void calibrateMaxMin(){
  tone(BuzzerPin, Buzzer_CalibrationHGIH);
  for(int loT = Buzzer_CalibrationHGIH; loT > Buzzer_CalibrationLOW; loT--){
    tone(BuzzerPin, loT);
    delay(Calibration_TimeToMiddle / (Buzzer_CalibrationHGIH - Buzzer_CalibrationLOW));
  }

  calibrateMiddle();
  
  tone(BuzzerPin, Buzzer_ShortTon);
  delay(Buzzer_ShortTone_Time);
  noTone(BuzzerPin);

  Serial.println("Wait for Input...");

  joystick_LX_max = joystick_LX_middle;
  joystick_LY_max = joystick_LY_middle;
  joystick_RX_max = joystick_RX_middle;
  joystick_RY_max = joystick_RY_middle;
  joystick_LX_min = joystick_LX_middle;
  joystick_LY_min = joystick_LY_middle;
  joystick_RX_min = joystick_RX_middle;
  joystick_RY_min = joystick_RY_middle;

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
    calibrateMaxMin();
  }
  else{
    tone(BuzzerPin, Buzzer_ShortTon);
    delay(2000);
    calibrateMiddle();
    noTone(BuzzerPin);

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

    if(buttonTime < Button_ShortPressTime){
      Code[CodeLenght] = 0;
      Serial.println("short press");
    }
    else if(buttonTime < Button_LongPressTime){
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

      if(buttonTime >= Button_LongPressTime){
        Finished = true;
        Serial.println("Code fineshed!");
      }
    }
  }

  if(!abort){
    Serial.println(String(Code[0]) + String(Code[1]) + String(Code[2]) + String(Code[3]) + String(Code[4]) + " Len:" + String(CodeLenght));
    if(Code[0] == 0 && Code[1] == 1 && Code[2] == 0 && Code[3] == 1 && Code[4] == 0  && CodeLenght == 4){
      Serial.println("Calibrate...");
      calibrateMaxMin();
    }
  }
}

void detachAttach(bool detach){
  if(detach){
    Drehung.detach();
    Arm.detach();
    Oberarm.detach();
    Hand.detach();

    detached = true;
  }
  else{
    Drehung.attach(DrehungPin);
    Arm.attach(ArmPin);
    Oberarm.attach(OberarmPin);
    Hand.attach(HandPin);

    detached = false;
  }
}

void Autoplay(){
  
}

int joystick_position(int joystick, int joystick_middle, int joystick_min, int joystick_max, int joystick_MinSpeed, int joystick_MaxSpeed){
  if(joystick <= joystick_middle - joystick_Empfindlichkeit)
    return(-map(joystick, joystick_middle - joystick_Empfindlichkeit, joystick_min, joystick_MinSpeed, joystick_MaxSpeed));
  else if(joystick >= joystick_middle + joystick_Empfindlichkeit)
    return(map(joystick, joystick_middle + joystick_Empfindlichkeit, joystick_max, joystick_MinSpeed, joystick_MaxSpeed));
  else
    return(0);
}

void loop(){
  joystick_LX = joystick_position(analogRead(joystick_LX_Pin), joystick_LX_middle, joystick_LX_min, joystick_LX_max, joystick_LX_MinSpeed, joystick_LX_MaxSpeed);
  joystick_LY = joystick_position(analogRead(joystick_LY_Pin), joystick_LY_middle, joystick_LY_min, joystick_LY_max, joystick_LY_MinSpeed, joystick_LY_MaxSpeed);
  joystick_RX = joystick_position(analogRead(joystick_RX_Pin), joystick_RX_middle, joystick_RX_min, joystick_RX_max, joystick_RX_MinSpeed, joystick_RX_MaxSpeed);
  joystick_RY = joystick_position(analogRead(joystick_RY_Pin), joystick_RY_middle, joystick_RY_min, joystick_RY_max, joystick_RY_MinSpeed, joystick_RY_MaxSpeed);

  if(joystick_LX == 0 && joystick_LY == 0 && joystick_RX == 0 && joystick_RY == 0){
    if (TimeToAutoplay > offtime)
      offtime++;
  }
  else{
    offtime = 0;
    if(detached){
      detachAttach(false);
    }

    if(joystick_LX != 0){
      if(InvertHand)HandPos -= joystick_LX;
      else HandPos += joystick_LX;
    }
    if(joystick_LY != 0){
      if(InvertOberarm)OberarmPos -= joystick_LY;
      else OberarmPos += joystick_LY;
    }
    if(joystick_RX != 0){
      if(InvertDrehung)DrehungPos -= joystick_RX;
      else DrehungPos += joystick_RX;
    }
    if(joystick_RY != 0){
      if(InvertArm)ArmPos -= joystick_RY;
      else ArmPos += joystick_RY;
    }
    
    
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

  if(offtime >= TimeToAutoplay){
    Autoplay();
  }
  else if(offtime >= TimeToDetach){
    detachAttach(true);
  }

  if(digitalRead(joystick_button_Pin) == LOW){
    detachAttach(true);
    joystickButtonPress();
  }

  Serial.println(String(offtime) + "," + String(DrehungPos) + "," + String(ArmPos) + "," + String(OberarmPos) + "," + String(HandPos));

  delay(LoopTime);
}