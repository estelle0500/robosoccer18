#include <compass.h>

float readAngle(){ 
  Serial2.flush();
  float out=Serial2.parseFloat();
  return out;
}

int anglePid(float val,float set, float gain, int maximum, int offset=20, int sqFactor=70){
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
