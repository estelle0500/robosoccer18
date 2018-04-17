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

void updateAll() {
    // Update sensors first
    cli();
    compass.readAngle();
    for (int i=0; i<4; i++) {
        ir_sensors[i]->readDistance();
	    //Serial.print(ir_sensors[i]->getDistance());
	    //Serial.print("\t");
    }
    //Serial.println();
    updateDriving();
    sei();
}

void initTimer() {
    // updateTimer.begin(updateAll, INTERVAL);
}
