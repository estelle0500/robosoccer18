import RPi.GPIO
import serial.tools.list_ports
import time
import serial
import struct
from serial import SerialException

class serialLink(object):

    def initSerial(self):
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
        if (not TeensyFound):
            print('Teensy Not Found')
        return TeensyFound, ser

    def __init__(self):
        TeensyFound, ser = self.initSerial()
        while (not TeensyFound):
            time.sleep(0.5)
            TeensyFound, ser = self.initSerial()
        self.ser = ser

    def handleSerialException():
        TeensyFound = False
        print('Teensy connection lost')
        counter = 0
        while( not TeensyFound):
            time.sleep(0.5)
            counter += 1
            print('Trying to re-establish connection. Try %d', counter)
            ports = list(serial.tools.list_ports.comports())
            for p in ports:
                print(p)
                # this needs to be changed according to the serial number of the specific Teensy
                if (p[2] == 'USB VID:PID=16c0:0483 SNR=4092670'):
                    print('Teensy Found')
                    TeensyFound = True
                    serialPortTeensy = p[0]
            # initialize the serial port when a Teensy is found among USB devices
            if (TeensyFound):
                self.ser = serial.Serial(
                    port=serialPortTeensy,
                    baudrate = 115200,
                    parity=serial.PARITY_NONE,
                    stopbits=serial.STOPBITS_ONE,
                    bytesize=serial.EIGHTBITS,
                    timeout=0.1
                )
                print('connection re-established')
                break
            if (not TeensyFound):
                print('failed')
            if (counter > 1000):
                print('we give up!')
                break

    def write(self, number):
        try:
            self.ser.write(struct.pack('>B',number))
        except SerialException:
            TeensyFound = False
            print('Teensy connection lost')
            counter = 0
            while( not TeensyFound):
                time.sleep(0.5)
                counter += 1
                print('Trying to re-establish connection. Try %d', counter)
                ports = list(serial.tools.list_ports.comports())
                for p in ports:
                    print(p)
                    # this needs to be changed according to the serial number of the specific Teensy
                    if (p[2] == 'USB VID:PID=16c0:0483 SNR=4092670'):
                        print('Teensy Found')
                        TeensyFound = True
                        serialPortTeensy = p[0]
                # initialize the serial port when a Teensy is found among USB devices
                if (TeensyFound):
                    self.ser = serial.Serial(
                        port=serialPortTeensy,
                        baudrate = 115200,
                        parity=serial.PARITY_NONE,
                        stopbits=serial.STOPBITS_ONE,
                        bytesize=serial.EIGHTBITS,
                        timeout=0.1
                    )
                    print('connection re-established')
                    break
                if (not TeensyFound):
                    print('failed')
                if (counter > 1000):
                    print('we give up!')
                    break

    def read(self, number):
        try:
            print(ord(self.ser.read()))
        except SerialException:
            TeensyFound = False
            print('Teensy connection lost')
            counter = 0
            while( not TeensyFound):
                time.sleep(0.5)
                counter += 1
                print('Trying to re-establish connection. Try %d', counter)
                ports = list(serial.tools.list_ports.comports())
                for p in ports:
                    print(p)
                    # this needs to be changed according to the serial number of the specific Teensy
                    if (p[2] == 'USB VID:PID=16c0:0483 SNR=4092670'):
                        print('Teensy Found')
                        TeensyFound = True
                        serialPortTeensy = p[0]
                # initialize the serial port when a Teensy is found among USB devices
                if (TeensyFound):
                    self.ser = serial.Serial(
                        port=serialPortTeensy,
                        baudrate = 115200,
                        parity=serial.PARITY_NONE,
                        stopbits=serial.STOPBITS_ONE,
                        bytesize=serial.EIGHTBITS,
                        timeout=0.1
                    )
                    print('connection re-established')
                    break
                if (not TeensyFound):
                    print('failed')
                if (counter > 1000):
                    print('we give up!')
                    break
