#include <Arduino.h>
#include <Servo.h>

#define servoDrehungPin 10
#define servoArmPin 7
#define servoOberarmPin 8
#define servoHandPin 9
#define poti1Pin 0
#define poti2Pin 5
#define poti3Pin 2
#define poti4Pin 3
#define loopTime 25
#define PotiFehlerBereich 100
#define AusschaltDelay 80

Servo servoDrehung, servoArm, servoOberarm, servoHand;

int Poti1, Poti2, Poti3, Poti4;
int servoDrehungPos, servoArmPos, servoOberarmPos, servoHandPos;
int servoDrehungPosOLD, servoArmPosOLD, servoOberarmPosOLD, servoHandPosOLD;
int offtime;

bool disabled = false;

void setup(){
  servoDrehung.attach(servoDrehungPin);
  servoArm.attach(servoArmPin);
  servoOberarm.attach(servoOberarmPin);
  servoHand.attach(servoHandPin);

  Serial.begin(9600);
}

void loop(){
  Poti1 = analogRead(poti1Pin);
  Poti2 = analogRead(poti2Pin);
  Poti3 = analogRead(poti3Pin);
  Poti4 = analogRead(poti4Pin);

  servoDrehungPos = map(Poti1, 0, 1023, 0, 180);
  servoArmPos = map(Poti2, 1023, 0, 0, 130);
  servoOberarmPos = map(Poti3, 0, 1023, 0, 180);
  servoHandPos = map(Poti4, 0, 1023, 45, 140);

  if(servoDrehungPos <= servoDrehungPosOLD + PotiFehlerBereich && servoDrehungPos >= servoDrehungPosOLD - PotiFehlerBereich && servoArmPos <= servoArmPosOLD + PotiFehlerBereich && servoArmPos >= servoArmPosOLD - PotiFehlerBereich && servoOberarmPos <= servoOberarmPosOLD + PotiFehlerBereich && servoOberarmPos >= servoOberarmPosOLD - PotiFehlerBereich && servoHandPos <= servoHandPosOLD + PotiFehlerBereich && servoHandPos >= servoHandPosOLD - PotiFehlerBereich){
    if(offtime < AusschaltDelay){
      offtime++;
    }
  }
  else{
    offtime = 0;

    servoDrehungPosOLD = servoDrehungPos;
    servoArmPosOLD = servoArmPos;
    servoOberarmPosOLD = servoOberarmPos;
    servoHandPosOLD = servoHandPos;
  }

  if(offtime >= AusschaltDelay){
    servoDrehung.detach();
    servoArm.detach();
    servoOberarm.detach();
    servoHand.detach();

    disabled = true;
  }
  else{
    if (disabled){
      servoDrehung.attach(servoDrehungPin);
      servoArm.attach(servoArmPin);
      servoOberarm.attach(servoOberarmPin);
      servoHand.attach(servoHandPin);

      disabled = false;
    }

    servoDrehung.write(servoDrehungPos);
    servoArm.write(servoArmPos);
    servoOberarm.write(servoOberarmPos);
    servoHand.write(servoHandPos);
  }

  Serial.println(String(offtime) + "," + String(servoDrehungPos) + "," + String(servoArmPos) + "," + String(servoOberarmPos) + "," + String(servoHandPos));

  delay(loopTime);
}