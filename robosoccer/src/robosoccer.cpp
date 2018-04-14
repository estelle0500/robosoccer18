#include "robosoccer.h"

#ifdef zejun
extern Motor se(DCHANNEL6,false); //D6
extern Motor ne(DCHANNEL5,false); //D5
extern Motor sw(DCHANNEL4,false); //D4
extern Motor nw(DCHANNEL3,false); //D3
#else
extern Motor se(DCHANNEL6,false); //D6
extern Motor ne(DCHANNEL5,false); //D5
extern Motor sw(DCHANNEL2,false); //D4
extern Motor nw(DCHANNEL1,false); //D3
#endif

extern Compass compass(Serial4, 115200);

extern volatile int DRIVE_DIR = 0;
extern volatile int CURR_SPD = 0;

extern Adafruit_ADS1115 ads(0x48);

// IRSensor ir_front(4,0x70), ir_back(0,0x70), ir_left(7,0x70), ir_right(2,0x70);
// extern IRSensor *ir_sensors[4] = {&ir_left, &ir_front, &ir_right, &ir_back};

LightSensor light_front(50, DIR_FRONT), light_left(13, DIR_LEFT), light_back(27, DIR_BACK), light_right(26, DIR_RIGHT);
extern LightSensor *light_sensors[4] = {&light_front, &light_left, &light_right, &light_back};

IntervalTimer updateTimer;

UltrasoundSensor ultra_front(0), ultra_left(0), ultra_right(0), ultra_back(0);
extern UltrasoundSensor *ultra_sensors[4] = {&ultra_front, &ultra_left, &ultra_right, &ultra_back};

void setup_components() {
    Wire1.begin();
    Wire1.beginTransmission(MULTIPLEXER_ADDRESS);
    Wire1.write(1<<1);
    Wire1.endTransmission();
    Serial.begin(115200);
    // for (int i=0; i<4; i++) ir_sensors[i]->init();
    for (int i=1; i<4; i++) light_sensors[i]->init(false);
    compass.init();
    ads.setGain(GAIN_ONE);
    updateTimer.begin(updateAll, INTERVAL);
}
