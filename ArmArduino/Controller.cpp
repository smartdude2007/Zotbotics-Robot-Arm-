#include "Coordinate.h"
#include "MotorPosition.h"
#include "Hand.h"
#include "math.h"
#include "Motor.h"
#include <AccelStepper.h>


#define M_PI 3.14159265358979323846
#define step1 35
#define dir1 37
#define step2 22
#define dir2 24
#define step3 30
#define dir3 28
#define step4B 45
#define dir4B 47
#define step5C 50 
#define dir5C 52

const int ENA_PIN = 44; // PWM-capable pin on Mega
const int IN1_PIN = 40;
const int IN2_PIN = 42;

                        //tune this number
#define L0 287.87      //length between motor 1 and 2
#define L1 260.97      //length between motor 2 and 3
#define L2 260.75      //length between motor 3 and 5

int16_t RxCoords[2];
Hand gripper;
float inputCoords[2];
int states;
int cnt;
Motor motors[5];
int timeIntervals[5];
unsigned long lastStepTime[5];
int minInterval;
int cupIdx;
unsigned long now;
Coordinate cupPosList[6];

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

  // pinMode(ENA_PIN, OUTPUT);
  // pinMode(IN1_PIN, OUTPUT);
  // pinMode(IN2_PIN, OUTPUT);
  // digitalWrite(ENA_PIN, HIGH);

  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, HIGH);
  digitalWrite(dir4B, LOW);
  digitalWrite(dir5C, HIGH); 
  digitalWrite(step1, LOW);
  digitalWrite(step2, LOW);
  digitalWrite(step3, LOW);
  digitalWrite(step4B, LOW);
  digitalWrite(step5C, LOW);
  
  states = 0;
  cupIdx = 0;
 
  motors[0] = Motor(step1, dir1, 1, 0);
  motors[1] = Motor(step2, dir2, 0, 1);
  motors[2] = Motor(step3, dir3, 1, 2); 
  motors[3] = Motor(step4B, dir4B, 0, 3); 
  motors[4] = Motor(step5C, dir5C, 1, 4); 

  Serial.begin(9600);
  Serial1.begin(9600);


  minInterval = 400;
  for (int i=0; i<5; i++){
    timeIntervals[i] = minInterval;
    lastStepTime[i] = 0;
  }
   // set the start coordinate

  Coordinate startPos(0,420, 355);
  gripper = Hand(InverseKine(startPos)); 

  delay(2000);

}

void loop() {
  
  // digitalWrite(dir, LOW);
  // digitalWrite(step4B, HIGH);
  // digitalWrite(step5C, HIGH);
  // delayMicroseconds(600);
  // digitalWrite(step4B, LOW);
  // digitalWrite(step5C, LOW);
  // delayMicroseconds(600);

  // Coordinate target(0, 420,200);
  // int x = moveTo(target);
  // if (x == 1){
  //   Serial.println(x);
  // }

  if (states == 0){
    if (readData()){
      Coordinate bottlePos(RxCoords[0], RxCoords[1], 100);

      if (moveTo(bottlePos)) {
        
        delay(500);
        states++;
        //grab the bottle
      }
    }
  }
  
  // else if (states == 1){
  //   if (readData()){
  //     Coordinate pourPos(RxCoords[0], RxCoords[1], 400);
  //     if (moveTo(pourPos)) {
  //       delay(500);
  //       states++;
  //       //move to pour position
  //     }
  //   }
  // }

  // else if (states == 2){
  //   //pour drink
  //   states++;
  // }
  
  // else if (state == 3){
  //   //place the bottle down and release the cup
  // }
}

// void loop2(){ // this is the loop for cup stacking
//   if (states == 0){ // move to grab position
//     if (readData()){
//       Coordinate grabPos(RxCoords[0], RxCoords[1], 400);
//       if (moveTo(grabPos)) {
//         delay(500);
//         states++;
//         //grab
//       }
//     }
//   }
//   else if (states == 1){
    
//     if (moveTo(cupPosList[cupIdx])){
//       delay(500);
//       cupIdx++;
//       states--;
//       if (cupIdx == 6){
//         states = 2;
//       }
//       //drop;
//     }
//   }
//   else if (states == 2){
//     //finish cup stacking
//   }
 



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


  
  long targetList[]= {(long)(objTheta*13.5/0.225), (long)(theta1*150/0.225), (long)(theta2*150/0.225), (long)(theta3*67.82/0.225), (long) (theta3*67.82/0.225)};
  // Serial.println((int)theta1*150/1.8);
  // Serial.println((int)theta2*150/1.8);
  // Serial.println((int)theta3*67.82/1.8);
  // Serial.println();
  MotorPosition targetPos(targetList);//change so that last angle makes wrist flat
  return targetPos;
}


int moveTo(Coordinate target){
  now = micros();
  int targetReached = 0;
  int cnt = 0;
  MotorPosition targetPosition = InverseKine(target);
  for (Motor m : motors){
    if ((abs(gripper.motorPos.coordState[m.idx] - targetPosition.coordState[m.idx])>3) && ((now - lastStepTime[m.idx]) > timeIntervals[m.idx])){
      setDirection(m, targetPosition.coordState[m.idx] - gripper.motorPos.coordState[m.idx]);
      move1(m);
      delayMicroseconds(5);
      stop1(m, targetPosition.coordState[m.idx] - gripper.motorPos.coordState[m.idx]);
      lastStepTime[m.idx] = now;
      long timeInteval = 1000 - 50L*abs((int)(targetPosition.coordState[m.idx] - gripper.motorPos.coordState[m.idx]));
      
      if (timeInteval < minInterval){
        timeInteval = minInterval;
      }
      
      timeIntervals[m.idx] = timeInteval;
    }
    else if (abs(gripper.motorPos.coordState[m.idx] - targetPosition.coordState[m.idx])<=5){
      cnt++;
      if (cnt == 5) targetReached = 1;
    }
  }
  return targetReached;  
}

void move1(Motor m){
   //Serial.println(m.stepPin);
   digitalWrite(m.stepPin, HIGH);
}

void stop1(Motor m, int direction){
  digitalWrite(m.stepPin, LOW);
  if (direction>0){
    gripper.motorPos.coordState[m.idx]++;
  }
  else if (direction<0){
    gripper.motorPos.coordState[m.idx]--;
  }
}

int readData(){
  if (Serial1.available() >= 5) {       // wait for full frame
    if (Serial1.read() == 0xFF) {       // check start marker
      byte b0 = Serial1.read();
      byte b1 = Serial1.read();
      byte b2 = Serial1.read();
      byte b3 = Serial1.read();

      RxCoords[0] = (b0 << 8) | b1;
      RxCoords[1] = (b2 << 8) | b3;
      RxCoords[1] += 250;
      return 1;
    }
    else{
      return 0;
    }
  }
  else {
    return 0;
  }
}

void setDirection(Motor m, int direction){
  if (direction > 0){
    //Serial.println("positive direction");
    if (m.defaultDirection == 1){
      digitalWrite(m.dirPin, HIGH);
      
    }
    else{
      digitalWrite(m.dirPin, LOW);
    }
    
  }
  else{
    //Serial.println("positive direction");
    if (m.defaultDirection == 1){
      digitalWrite(m.dirPin, LOW);
    }
    else{
      digitalWrite(m.dirPin, HIGH);
    }
  }
}
