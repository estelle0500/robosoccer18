# robosoccer18

Code repository for Robosoccer 2018

## Hardware Setup

**Teensy 3.5**: Wire it as specified in the [circuit drawing](https://drive.google.com/drive/folders/1_fFdSYqlDCo2Nxi7MjGL3PBpwIonidsW?usp=sharing). Otherwise, you may need to change the pin mappings in the code.
**Raspberry Pi**: Nothing yet.

## Software Setup

**Teensy 3.5**
1. Download the [Arduino IDE](https://www.arduino.cc/en/Main/Software) and [software for Teensy](https://www.pjrc.com/teensy/td_download.html).
2. Download and install each folder in **external_libs** as a separate library in Arduino IDE.
3. Download and install **source_libs** as one library and name it **robosoccer** in Arduino IDE.
4. Edit or create new files for Arduino code (.ino) in the **main** folder.

**Raspberry Pi**
1. Download OpenCV 3.3.0 for Python. 
2. Will write this eventually

## TODO
Might want to minimise direct edits to the library and instead write wrappers as part of source_libs.

