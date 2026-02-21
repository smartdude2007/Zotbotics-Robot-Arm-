#ifndef COORDINATE_H
#define COORDINATE_H

class Coordinate{
  public: 
    Coordinate(){
      this->x = 0;
      this-> y = 0;
      this-> z = 0;
    }
    Coordinate(float x, float y, float z){
      this->x  = x;
      this->y = y;
      this-> z = z;
    };

    void updateCoord(float x, float y, float z){
      this->x  = x;
      this->y = y;
      this-> z = z;
    };

    float x;
    float y;
    float z;
};

#endif
