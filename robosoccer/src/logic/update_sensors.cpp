#include "update_sensors.h"

void updateDriving() {
    int speeds[4];
    computeDrive(DRIVE_DIR, CURR_SPD, compass.getAngle(),
                 compass.getNorth(), MAX_TURN, speeds);
    /*for (int i=0; i<4; i++) {
        Serial.print(speeds[i]);
        Serial.print("\t");
    }
    Serial.println();*/
    if (CURR_SPD != 0) vroom(speeds);
    else brake();
}

int deg_dirs[4] = {180, 90, -90, 0};
volatile int countdown = 0;
volatile int trig = 0;

void updateAll() {
    // Update sensors first
    cli();
    compass.readAngle();
    /*for (int i=0; i<4; i++) {
        ir_sensors[i]->readDistance();
	    //Serial.print(ir_sensors[i]->getDistance());
	    //Serial.print("\t");
    }*/
    //Serial.println();
    if (countdown==0) {
        for (int i=1; i<4; i++) {
            if (light_sensors[i]->triggered()) {
                trig=i;
                CURR_SPD = MAX_SPD;
                DRIVE_DIR = deg_dirs[i];
                countdown=25;
                break;
            }
        }
    }
    else countdown--;
    updateDriving();
    sei();
}

void initTimer() {
    // updateTimer.begin(updateAll, INTERVAL);
}
