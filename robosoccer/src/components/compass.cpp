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

bool Compass::readAngle(){
  if ((this->serial)->available()>2) {
      bool sign;
      while ((this->serial)->available()) {
          byte start = (this->serial)->read();
          if (start>=254) {
              sign = 255 ^ start;
              // start byte is 255 means negative number
              // 254 means positive number
              // start byte should not be anything else
              break;
          }
      }
      if ((this->serial)->available()) {
          byte data = (this->serial)->read();
          if (data <= 180) {
              // larger than 180 must be a bad reading
              if (sign) this->angle = data;
              else this->angle = -data;
          }

          return true;
      }
  }
  return false;
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
