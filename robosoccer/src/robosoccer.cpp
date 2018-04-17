#include "robosoccer.h"

#ifdef zejun
extern Motor se(DCHANNEL6,false); //D6
extern Motor ne(DCHANNEL5,false); //D5
extern Motor sw(DCHANNEL4,false); //D4
extern Motor nw(DCHANNEL3,false); //D3
#else
extern Motor ne(DCHANNEL6,false); //D6
extern Motor se(DCHANNEL5,true); //D5
extern Motor nw(DCHANNEL2,true); //D4
extern Motor sw(DCHANNEL1,true); //D3
#endif

Compass compass(Serial4, 115200);

volatile int DRIVE_DIR = 0;
volatile int CURR_SPD = 0;

Adafruit_ADS1115 ads(0x48);

// IRSensor ir_front(4,0x70), ir_back(0,0x70), ir_left(7,0x70), ir_right(2,0x70);
// extern IRSensor *ir_sensors[4] = {&ir_left, &ir_front, &ir_right, &ir_back};

LightSensor light_front(50, DIR_FRONT), light_left(13, DIR_LEFT), light_back(27, DIR_BACK), light_right(26, DIR_RIGHT);
LightSensor *light_sensors[4] = {&light_front, &light_left, &light_right, &light_back};

UltrasoundSensor ultra_front(2, ads), ultra_left(3, ads), ultra_right(0, ads), ultra_back(0, ads);
UltrasoundSensor *ultra_sensors[4] = {&ultra_front, &ultra_left, &ultra_right, &ultra_back};

RpiCommunication communication;
IntervalTimer updateTimer;

void setupComponents() {
    Wire1.begin();
    Serial.begin(115200);
    // for (int i=0; i<4; i++) ir_sensors[i]->init();
    for (int i=1; i<4; i++) light_sensors[i]->init();
    compass.init();
    ads.useMultiplexer();
    ads.setGain(GAIN_ONE);
    ads.begin();
    updateTimer.begin(updateAll, INTERVAL);
}

bool ball_detected = false, has_ball = false;
int ball_angle = 0, ball_distance = -1;
int no_ball_count = 0;
int looping_direction = NO_DIRECTION;

void waitKey() {
  // type q to stop the robot from running
  // type r to restart the robot
  while (Serial.available()) {
    byte b = Serial.read();
    if (b == 'q') {
      bool restarted = false;
      while (!restarted) {
        if (Serial.available()) {
          byte b = Serial.read();
          if (b == 'r') restarted=true;
        }
      }
    }
  }
}
