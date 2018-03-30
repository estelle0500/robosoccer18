import csv
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

def waitForSerial(ser, min_bytes=4):
    while (ser.in_waiting<min_bytes):
        pass

def receiveLong(ser):
    waitForSerial(ser)
    bytes_received = ser.read(4)
    long_number = struct.unpack('l', bytes_received)
    return long_number

def receiveFloat(ser):
    waitForSerial(ser)
    bytes_received = ser.read(4)
    float_number = struct.unpack('f', bytes_received)
    return float_number

def receiveMessage(ser, startChar=b'#'):
    startMessage = False
    while not startMessage:
        if ser.in_waiting():
            c = ser.read(1)
            if c == startChar:
                startMessage = True

    waitForSerial(ser, 20)
    bytes_received = ser.read(20)
    message = struct.unpack('lffff', bytes_received)

    return message

def writeTrackingData(ser, filename="tracked_sensor_data.csv"):
    with open(filename, "w") as csvfile:
        data_writer = csv.writer(csvfile)
        message = receiveMessage(ser)
        data_writer.writerow(list(message))

try:
    ser = initSerial('/dev/ttyS0')
    writeTrackingData(ser)
finally:
    # Do something, idk
    pass
