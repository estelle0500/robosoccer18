# robosoccer18

Code repository for Robosoccer 2018

## Hardware Setup

**Teensy 3.5**: Wire it as specified in the [circuit drawing](https://drive.google.com/drive/folders/1_fFdSYqlDCo2Nxi7MjGL3PBpwIonidsW?usp=sharing). Otherwise, you may need to change the pin mappings in the code.
**Raspberry Pi**: Nothing yet.

## Software Setup

**Teensy 3.5**
1. Download the [Arduino IDE](https://www.arduino.cc/en/Main/Software) and [software for Teensy](https://www.pjrc.com/teensy/td_download.html).
2. Download and install **robosoccer** as a library in Arduino IDE (you must have Arduino IDE version 1.6.0 or newer).
3. Edit or create new files for Arduino code (.ino) in the **main** folder.

**Raspberry Pi**
1. Download OpenCV 3.3.0 for Python. 
2. Will write this eventually

## TODO

1. Rewrite IR sensor code as a wrapper in *source_libs* instead of directly editing the VL53L0X library
2. Rewrite movement code into a class
3. Incorporate the code in *tests* to the corresponding classes 
4. Move all the current code into a folder for Teensy code (so that it won't be confusing once we add in Raspberry Pi code)
5. (Maybe?) Make all the code directly refer to *external_libs* so that those libraries don't need to be installed one by one...
