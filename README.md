# robosoccer18

Code repository for Robosoccer 2018

## Hardware Setup

**Teensy 3.5**: Wire it as specified in the [circuit drawing](https://drive.google.com/drive/folders/1_fFdSYqlDCo2Nxi7MjGL3PBpwIonidsW?usp=sharing). Otherwise, you may need to change the pin mappings in the code.

| Component | Connection Type |
|:--------- |:---------------:|
| Compass   | Serial4         |
| Multiplexer | Address 0x70 on Wire1 |
| Front IR Sensor | Address 0x29, Port 4 on multiplexer |
| Left IR Sensor | Address 0x29, Port 7 on multiplexer |
| Back IR Sensor | Address 0x29, Port 0 on multiplexer |
| Right IR Sensor | Address 0x29, Port 2 on multiplexer |

**Raspberry Pi**: Connected to Teensy via USB cable.

## Software Setup

**Teensy 3.5**
1. Download the [Arduino IDE](https://www.arduino.cc/en/Main/Software) and [software for Teensy](https://www.pjrc.com/teensy/td_download.html).
2. Download and install **robosoccer** as a library in Arduino IDE (you must have Arduino IDE version 1.6.0 or newer).
3. Edit or create new files for Arduino code (.ino) in the **main** folder.

**Raspberry Pi**
1. Download OpenCV 3.3.0 for Python. 
2. Will write this eventually

## TODO

1. Write and test localization code.
2. Write and test out-of-bounds code.
3. Test ball tracking code.
4. Integrate the strategies.
