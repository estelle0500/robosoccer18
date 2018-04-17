#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include "../external_libs/VL53L0X/VL53L0X.h"
#include "../external_libs/i2c_t3/i2c_t3.h"

class IRSensor : public VL53L0X {
private:
    uint16_t distance;
    uint8_t port_num, multiplexer_addr;
public:
    IRSensor (uint8_t port_num, uint8_t multiplexer_addr);
    void useMultiplexer();
    void init(uint32_t period_ms = 0);

    // readDistance should return true if distance succesfully updated
    // and false otherwise
    bool readDistance();
    inline uint16_t getDistance() {
        return this->distance;
    };
};

#endif
