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
public:
    LightSensor(uint8_t pin_number, uint8_t direction);
    void init();
    inline bool triggered() {
        return digitalRead(this->pin_number)==HIGH;
    }
};

#endif
