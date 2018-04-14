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

bool alt = true;
void updateAll() {
    // Update sensors first
    cli();
    compass.readAngle();
    if (alt) {
        for (int i=0; i<4; i++) {
            ultra_sensors[i]->readDistance();
            // ir_sensors[i]->readDistance();
        }
    }
    alt = !alt; // this allows ultrasound sensor reading to occur only on alternate updates
    updateDriving();
    sei();
}

void initTimer() {
    // updateTimer.begin(updateAll, INTERVAL);
}
