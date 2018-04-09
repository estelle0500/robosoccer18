import RPi.GPIO
import time
import serial
import struct

def initSerial(port='/dev/ttyS0'):
    ser = serial.Serial(
        port='/dev/ttyS0',
        baudrate = 115200,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=0.1
    )
    return ser

def sendFloat(ser, fpnum):
    arr = bytearray(struct.pack("f", fpnum))
    ser.write(arr)
