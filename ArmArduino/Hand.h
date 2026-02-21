#ifndef HAND_H
#define HAND_H
#include "Coordinate.h"
#include "MotorPosition.h"

class Hand{
  public:
    
     Hand(MotorPosition start) : motorPos(start) {
      // motorPos is now initialized directly, no default constructor needed
    };
    Hand(){}
    MotorPosition motorPos;
};

#endif
