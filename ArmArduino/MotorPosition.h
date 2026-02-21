#ifndef MOTORPOSITION_H
#define MOTORPOSITION_H


class MotorPosition{
  public: 
    float coordState[5];
    MotorPosition(){
      this->coordState[0] = 0;
      this->coordState[1] = 0;
      this->coordState[2] = 0;
      this->coordState[3] = 0;
      this->coordState[4] = 0;
      this->coordState[5] = 0;

    }
    MotorPosition(float CS[]){
      this->coordState[0] = CS[0];
      this->coordState[1] = CS[1];
      this->coordState[2] = CS[2];
      this->coordState[3] = CS[3];
      this->coordState[4] = CS[4];
      this->coordState[5] = CS[5];

    
    };
    
};

#endif
