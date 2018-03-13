#include "compass.h"


Compass::Compass(HardwareSerial& serial, int baud_rate) {
    this->serial = &serial;
    this->baud_rate = baud_rate;
}

void Compass::init(){
    (this->serial)->begin(this->baud_rate);
    delay(300);
    this->north = this->readAngle();
}

float Compass::readAngle(){
  (this->serial)->flush();
  float out=(this->serial)->parseFloat();
  return out;
}

int anglePid(float val,float set, float gain, int maximum, int offset, int sqFactor){
  float e=(val-set);
  if(e>=180){
    e=e-360;
  }
  else if(e<=-180){
    e=360+e;
  }
  int x=e*gain+e*abs(e/sqFactor)*gain;
  if(x<0){
    x-=offset;
  }
  if(x>0){
    x+=offset;
  }
  if(x>maximum){
    return maximum;
  }
  if(x<-maximum){
    return -maximum;
  }
  return x;
}
