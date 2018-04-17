#include "update_sensors.h"

volatile bool blocked_dir[4] = {0, 0, 0, 0}; // front, left, back, right
volatile bool disallow_quad[4] = {0, 0, 0, 0}; // nw, ne, se, sw quadrant
volatile int countdown = 0;
volatile bool alt = true;
volatile int trig = -1;

void filterDriveDir() {
    if (countdown) {
        DRIVE_DIR = deg_dirs[trig];
        return;
    }
    else trig = -1;

    int move_quad = 0;
    if (DRIVE_DIR>=90) move_quad = 2;
    else if (DRIVE_DIR>0) move_quad = 1;
    else if (DRIVE_DIR<=-90) move_quad = 3;

    if (disallow_quad[move_quad]) {
        if (disallow_quad[(move_quad+3)%4] && disallow_quad[(move_quad+1)%4]) {
            // nowhere to move
            // e.g. robot wants to move nw but sw, nw, ne quads blocked
            CURR_SPD = 0;
        }
        else if (disallow_quad[(move_quad+3)%4]) {
            // e.g. robot wants to move nw but nw, sw quads blocked --> move forward
            DRIVE_DIR = deg_dirs_cw[move_quad];
        }
        else if (disallow_quad[(move_quad+1)%4]) {
            // e.g. robot wants to move nw but nw, ne quads blocked --> move left
            DRIVE_DIR = deg_dirs_cw[(move_quad+3)%4];
        }
    }
}

void updateDriving() {
    int speeds[4];
    filterDriveDir();
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
    cli();
    // Update compass reading
    compass.readAngle();
    // Check light sensors
    for (int i=1; i<4; i++) {
        if (light_sensors[i]->triggered() && !blocked_dir[i] && trig==-1) {
            // Triggered a sensor that hasn't been recently Triggered:
            // Try to move away
            blocked_dir[i] = true;
            countdown = 5;
            trig = i;
            break;
        }
        else if (light_sensors[i]->triggered() && blocked_dir[i]) {
            // Sensor is still triggered:
            // Continue trying to move away
            countdown--;
        }
        else if (!light_sensors[i]->triggered() && blocked_dir[i]) {
            // Sensor on this side recently triggered, but no longer detecting line:
            // DO NOT clear flag for recently triggered
            // But stop trying to move out.
            countdown = 0; // anything goes
        }
    }
    // Update distance (ultrasound) sensor readings
    if (alt) {
        ads.useMultiplexer();
        for (int i=0; i<4; i++) {
            ultra_sensors[i]->readDistance();
            if (ultra_sensors[i]->getDistance()>45) {
                blocked_dir[i] = false;
            }
            // ir_sensors[i]->readDistance();
        }
    }
    alt = !alt; // this allows ultrasound sensor reading to occur only on alternate updates
    // Update which quadrants are available
    disallow_quad[0] = blocked_dir[0] | blocked_dir[1];
    disallow_quad[1] = blocked_dir[0] | blocked_dir[3];
    disallow_quad[2] = blocked_dir[2] | blocked_dir[1];
    disallow_quad[3] = blocked_dir[2] | blocked_dir[3];
    updateDriving();
    sei();
}
