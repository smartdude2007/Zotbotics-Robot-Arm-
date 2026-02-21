#ifndef MOTOR_H
#define MOTOR_H

class Motor{
  public:
    Motor(){
      
    };
    Motor(int stepPin, int dirPin, int defualtDirection, int idx){
      this -> stepPin = stepPin;
      this -> dirPin = dirPin;
      this -> defaultDirection = defualtDirection;
      this -> idx = idx;
    };
  
    int stepPin;
    int dirPin;
    int defaultDirection;
    int idx;

};

#endif
