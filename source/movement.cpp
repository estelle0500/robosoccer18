#include <movement.h>

void init_drive() {
  // put your setup code here, to run once:
/* TCCR1B=0;
  TCCR1B|= (1 << CS11 || 1 << CS10);
  TCCR2B=0;
  TCCR2B|= (1 << CS11 || 1 << CS10);
  TCCR3B=0;
  TCCR3B|= (1 << CS11 || 1 << CS10);
  TCCR4B=0;
  TCCR4B|= (1 << CS11 || 1 << CS10);*/
  delay(300);
  north=readAngle();
  return;
}

void computeDrive(float drvDir, float maxSpd, float currAngle, float alignAngle, float maxTurn){
/* Calculates speeds for all wheels
/0  1\

\2  3/ Clockwise=+ve
*/
  maxSpd=max(maxSpd-DRIVE_OFFSET,0);
  while(drvDir>180){
      drvDir-=360;
  }
  while(drvDir<-180){
    drvDir+=360;
  }
  
  float x=-SpeedTrig.cos((drvDir-45));
  float y=SpeedTrig.sin((drvDir-45));
  float norm;
  if(max(abs(x),abs(y))==0){
    norm=0;
  }
  else{
    norm=maxSpd/max(abs(x),abs(y));
  }
  int a=x*norm;
  int b=y*norm;
  int diff=abs(abs(a)-abs(b));
  int turnPwr=anglePid(currAngle,alignAngle,ALIGN_GAIN_C+ALIGN_GAIN_M*maxSpd,max(diff,maxTurn),0,256);
  int cutPwr=0;
  int addPwr=0;
//  Serial.print(turnPwr);Serial.print("\t");Serial.print(currAngle);Serial.print("\t");Serial.println(alignAngle);
  if(turnPwr>diff){
    cutPwr=turnPwr-diff;
    turnPwr=diff;
  }
  else if(-turnPwr>diff){
    cutPwr=turnPwr+diff;
    turnPwr=-diff;
  }
  if(cutPwr>2*(maxSpd-diff)){
    addPwr=cutPwr-2*(maxSpd-diff);
    cutPwr=2*(maxSpd-diff);
  }
  else if(-cutPwr>2*(maxSpd-diff)){
    addPwr=cutPwr+2*(maxSpd-diff);
    cutPwr=-2*(maxSpd-diff);
  }
  if(abs(a)<abs(b)){ //modify a power to turn
    speeds[0]=a+turnPwr;
    speeds[1]=-b;
    speeds[2]=-a+turnPwr;
    speeds[3]=b;
  }
  else{ //modify b power to turn
    speeds[0]=a;
    speeds[1]=-b+turnPwr;
    speeds[2]=-a;
    speeds[3]=b+turnPwr;
  }
  speeds[0]+=addPwr;
  speeds[1]+=addPwr;
  speeds[2]+=addPwr;
  speeds[3]+=addPwr;
  // cut motor powers to turn
  if(a>0 && b>0){
    if(cutPwr>0){ //cut west motors (1,2)
      speeds[1]+=cutPwr;
      speeds[2]+=cutPwr;
    }
    else if (cutPwr<0){ //cut east motors (0,3)
      speeds[0]+=cutPwr;
      speeds[3]+=cutPwr;
    }
  }
  else if(a>0 && b<0){
    if(cutPwr>0){ //cut back motors (2,3)
      speeds[2]+=cutPwr;
      speeds[3]+=cutPwr;
    }
    else if (cutPwr<0){ //cut front motors (0,1)
      speeds[0]+=cutPwr;
      speeds[1]+=cutPwr;
    }
  }
  else if(a<0 && b<0){
    if(cutPwr<0){ //cut west motors (1,2)
      speeds[1]+=cutPwr;
      speeds[2]+=cutPwr;
    }
    else if (cutPwr>0){ //cut east motors (0,3)
      speeds[0]+=cutPwr;
      speeds[3]+=cutPwr;
    }
  }
  else if(a<0 && b>0){
    if(cutPwr<0){ //cut back motors (2,3)
      speeds[2]+=cutPwr;
      speeds[3]+=cutPwr;
    }
    else if (cutPwr>0){ //cut front motors (0,1)
      speeds[0]+=cutPwr;
      speeds[1]+=cutPwr;
    }
  }
  speeds[0]+=DRIVE_OFFSET*sign(speeds[0]);
  speeds[1]+=DRIVE_OFFSET*sign(speeds[1]);
  speeds[2]+=DRIVE_OFFSET*sign(speeds[2]);
  speeds[3]+=DRIVE_OFFSET*sign(speeds[3]);
  return;
}

void vroom(){
  nw.move(speeds[0]);
  ne.move(speeds[1]);
  se.move(speeds[2]);
  sw.move(speeds[3]);
  return;
}

void brake(){
  nw.move(0);
  ne.move(0);
  se.move(0);
  sw.move(0);
  return;
}


