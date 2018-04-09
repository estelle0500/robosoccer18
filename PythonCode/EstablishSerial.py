import RPi.GPIO
import serial.tools.list_ports
import time
import serial

def initSerial():
    ser = 0
    TeensyFound = False
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        print(p)
        # this needs to be changed according to the serial number of the specific Teensy
        if (p[2] == 'USB VID:PID=16c0:0483 SNR=4092670'):
            print('Teensy Found')
            serialPortTeensy = p[0]
            TeensyFound = True

    # initialize the serial port when a Teensy is found among USB devices
    if (TeensyFound):
        ser = serial.Serial(
            port=serialPortTeensy,
            baudrate = 115200,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            timeout=0.1
        )
        print('port open')
    # just something to make the program end gracefully if no Teensy is found
    if (not TeensyFound):
        print('Teensy Not Found')

    return TeensyFound, ser

