#include "Coordinate.h"
#include "MotorPosition.h"
#include "Hand.h"
#include "math.h"
#include "Motor.h"
#include <AccelStepper.h>


#define M_PI 3.14159265358979323846
#define step1 31
#define dir1 33
#define step2 22
#define dir2 24
#define step3 48
#define dir3 46
#define step4B 36
#define dir4B 34
#define step5C 49
#define dir5C 51

                        //tune this number
#define L0 287.87      //length between motor 1 and 2
#define L1 260.97      //length between motor 2 and 3
#define L2 260.75      //length between motor 3 and 5


Hand gripper;
float inputCoords[2];
int states;
Motor motors[5];

void setup() {
  pinMode(step1, OUTPUT);
  pinMode(step2, OUTPUT);
  pinMode(step3, OUTPUT);
  pinMode(step4B, OUTPUT);
  pinMode(step5C, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2,OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(dir4B, OUTPUT);
  pinMode(dir5C, OUTPUT);

  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, HIGH);
  digitalWrite(dir4B, HIGH);
  digitalWrite(dir5C, HIGH);
  digitalWrite(step1, LOW);
  digitalWrite(step2, LOW);
  digitalWrite(step3, LOW);
  digitalWrite(step4B, LOW);
  digitalWrite(step5C, LOW);




  
  states = 0;
 
  motors[0] = Motor(step1, dir1, 1, 0);
  motors[1] = Motor(step2, dir2, 1, 1);
  motors[2] = Motor(step3, dir3, 1, 2);
  motors[3] = Motor(step4B, dir4B, 1, 3);
  motors[4] = Motor(step5C, dir5C, 1, 4);

  Serial.begin(9600);
   // set the start coordinate

  // Coordinate startPos;
  // Hand gripper(InverseKine(startPos)); 
  // Coordinate target(300, 300, 100);
  // MotorPosition x = InverseKine(target);   
  delayMicroseconds(3000);
}

void loop() { // this is the loop for bottle pouring

    digitalWrite(step2, HIGH);
    delayMicroseconds(800);
    digitalWrite(step2, LOW);
    delayMicroseconds(800);

  if (states == 0){
    // Serial.readBytes((byte*)inputCoords, sizeof(inputrCoords));
  

    // Coordinate target(inputCoords[0], inputCoords[1], 50); // input coordinates from rasberry pi

    // moveTo(gripper, target); 

    
  }
  
  else if (states == 1){
    // Serial.readBytes((byte*)inputCoords, sizeof(inputrCoords));
  

    // Coordinate target(inputCoords[0]+100, inputCoords[1], 100); // input coordinates from rasberry pi

    // moveTo(gripper, target);
  }

  else if (states == 2){
    //rotate hands
  }



}

void loop2(){ // this is the loop for cup stacking
  if (states == 0){
    // Serial.readBytes((byte*)inputCoords, sizeof(inputrCoords));
  

    // Coordinate target(inputCoords[0], inputCoords[1], 60); // input coordinates from rasberry pi

    // moveTo(gripper, target);
  }
  else if (states == 1){
    //move to the cup placement
  }
  else{
    //drop cut
    //after some time, reset states to 0
  }
}


MotorPosition InverseKine(Coordinate objUnfiltered) { // treat the forward axis as y, left right as x. Angle 0 is the Y AXIS not the x axis. To the right of the Y-axis is positive angle, left is negative 
  float r = sqrt(pow(objUnfiltered.x,2) + pow(objUnfiltered.y, 2)); 
  Coordinate obj(objUnfiltered.x * (r - 74.74)/r, objUnfiltered.y * (r-74.74)/r, objUnfiltered.z);

  float objTheta = (atan(obj.x/obj.y)) * 180.0/M_PI;  // this is top down view; this is to rotate the base to face the right direction

  //inverseKine simplifies to a 2d problem
  float distance = sqrt(pow(obj.x, 2) + pow(obj.y - 20.25,2) + pow(L0-obj.z, 2)); 
  float distance2 = sqrt(pow(obj.x,2) + pow(obj.y - 20.25 , 2) + pow(obj.z, 2));

  float theta1 = 180/M_PI * (acos((pow(distance, 2) + pow(L1, 2) - pow(L2, 2))/(2*L1*distance)) + acos((pow(distance,2)+pow(L0, 2)-pow(distance2,2))/(2*distance*L0)));

  float theta2 = 180/M_PI * (acos((pow(L1,2)+pow(L2,2)-pow(distance,2))/(2*L1*L1)));

  float theta3 = theta1+theta2 -90;
  
  Serial.println(objTheta);
  Serial.println(theta1);
  Serial.println(theta2);
  Serial.println(theta3);

  float targetList[]= {objTheta*13.5/1.8, theta1*150/1.8, theta2*150/1.8, theta3*67.82/1.8, theta3*67.82/1.8, 180};
  MotorPosition targetPos(targetList);//change so that last angle makes wrist flat
  return targetPos;

}

// void Reset(Hand h){ //reset the hand, and resets its coord and motor-position

// }



// void moveToLoop(Coordinate target, Hand gripper){
//   MotorPosition targetPositions = InverseKine(target);
//   int limit1 = abs(targetPositions.coordState[0]- gripper.motorPos.coordState[0]);
//   int limit2 = abs(targetPositions.coordState[1]- gripper.motorPos.coordState[1]);
//   int limit3 = abs(targetPositions.coordState[2]- gripper.motorPos.coordState[2]);
//   int limit4 = abs(targetPositions.coordState[3]- gripper.motorPos.coordState[3]);
//   int limit5 = abs(targetPositions.coordState[4]- gripper.motorPos.coordState[4]);

//   for (int i = 0; i < max(limit1, max(limit2, max(limit3, limit4))); i++){
//     if (i<limit1){
//       set motor high
//     }
//      if (i<limit2){
//       set motor high
//     }
//      if (i<limit3){
//       set motor high
//     }
//      if (i<limit4){
//       set motor high
//     }
//     delay(200);
//     if (i<limit1){
//       set motor low
//       update motor
//     }
//      if (i<limit2){
//       set motor low
//       update motor
//     }
//      if (i<limit3){
//       set motor low
//       update motor
//     }
//      if (i<limit4){
//       set motor low
//       update motor
//     }
//     delay(200)
//   }
// }

void moveTo(Hand gripper, Coordinate target){
  MotorPosition targetPosition = InverseKine(target);

  for (Motor m : motors){
    if (abs(gripper.motorPos.coordState[m.idx] - targetPosition.coordState[m.idx])>5){
      setDirection(m, targetPosition.coordState[m.idx] - gripper.motorPos.coordState[m.idx]);
      move1(m);
    }
  }
  delay(400);
  for (Motor m: motors){
    if (abs(gripper.motorPos.coordState[m.idx] - targetPosition.coordState[m.idx])>5){
      stop1(gripper, m, targetPosition.coordState[m.idx] - gripper.motorPos.coordState[m.idx]);
    }
  }
  delay(400);
}

void move1(Motor m){
   digitalWrite(m.stepPin, HIGH);
}

void stop1(Hand gripper, Motor m, int direction){
  digitalWrite(m.stepPin, LOW);
  if (direction>0){
    gripper.motorPos.coordState[m.idx]++;
  }
  else if (direction<0){
    gripper.motorPos.coordState[m.idx]--;
  }
  
}


void setDirection(Motor m, int direction){
  if (direction > 0){
    digitalWrite(m.dirPin, m.defaultDirection);
  }
  else{
    digitalWrite(m.dirPin, !m.defaultDirection);
  }
}





