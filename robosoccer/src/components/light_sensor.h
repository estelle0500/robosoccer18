#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#define DIR_FRONT 0
#define DIR_RIGHT 1
#define DIR_BACK 2
#define DIR_LEFT 3

#define MIN_INTERRUPT_INTERVAL 50

class LightSensor {
private:
    uint8_t pin_number, direction;
    static void avoidLine(uint8_t direction);
    static void avoidLineFront();
    static void avoidLineRight();
    static void avoidLineBack();
    static void avoidLineLeft();
public:
    LightSensor(uint8_t pin_number, uint8_t direction);
    void init(bool interrupt_enabled = true);
    bool triggered() {
        return digitalRead(this->pin_number)==HIGH;
    }
};

#endif
