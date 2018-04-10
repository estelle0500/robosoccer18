#include "ir_sensor.h"

IRSensor::IRSensor(uint8_t port_num, uint8_t multiplexer_addr) : VL53L0X() {
    this->port_num = port_num;
    this->multiplexer_addr = multiplexer_addr;
}

void IRSensor::useMultiplexer() {
    Wire1.beginTransmission(multiplexer_addr);
    Wire1.write(1<<port_num);
    Wire1.endTransmission();
}



bool IRSensor::readDistance() {
    if ((readReg(RESULT_INTERRUPT_STATUS) & 0x07) == 0) return false;

    this->distance = readReg16Bit(RESULT_RANGE_STATUS + 10);
    writeReg(SYSTEM_INTERRUPT_CLEAR, 0x01);
    return true;
}
