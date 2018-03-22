#include "Arduino.h"
#include "Dribbler.h"
#include "Motor.h"


Dribbler::Dribbler(int left[4], int right[4]){
  leftDribbler.init(left[0],left[1],left[2],left[3],false); 
  rightDribbler.init(right[0],right[1],right[2],right[3],false); 
}

void Dribbler::on(int velocity){
  leftDribbler.move(velocity);
  rightDribbler.move(velocity);
}

void Dribbler::off(){
  leftDribbler.move(0);
  rightDribbler.move(0);
}
