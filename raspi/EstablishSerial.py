#import RPi.GPIO
import serial.tools.list_ports
import time
import serial
import struct

class serialLink(object):

    def __init__(self):
        TeensyFound, ser = self.initSerial()
        while (not TeensyFound):
            time.sleep(500)
            TeensyFound, ser = self.initSerial()
        self.ser = ser
        self.buffer = bytes([])


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
        # just something to make the program end gracefully if no Teensy is found
        if (not TeensyFound):
            print('Teensy Not Found')
        return TeensyFound, ser

    def handleSerialException(self):
        TeensyFound = False
        print('Teensy connection lost')
        counter = 0
        while( not TeensyFound):
            time.sleep(500)
            counter = counter + 1
            print('Trying to re-establish connection. Try %d',counter)
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
            # just something to make the program end gracefully if no Teensy is found
            if (not TeensyFound):
                print('failed')
            if (counter > 1000):
                print('we give up!')
                break

    def add(self,number):
        self.buffer = self.buffer+struct.pack(b'B',number)

    def transmit(self):
        try:
            self.ser.write(self.buffer)
            self.buffer = self.buffer = bytes([])
        except:
            self.handleSerialException()

    def debugRead(self):
        while(self.ser.inWaiting()>0):
            try:
                print(ord(self.ser.read()))
            except:
                self.handleSerialException()

    def addDetectedObject(self, identifier, angle, distance):
        identifierByte = 200 + identifier
        if (angle >= 0):
            identifierByte = identifierByte + 10
        angleByte = angle
        if (angle < 0):
            angleByte = -angle
        distanceByte = distance // 2
        self.add(identifierByte)
        self.add(angleByte)
        self.add(distanceByte)


communication = serialLink()
communication.add(255)
communication.addDetectedObject(1,140,250)
communication.addDetectedObject(5,-110,100)
communication.add(254)
communication.add(255)
communication.addDetectedObject(1,140,250)
communication.addDetectedObject(5,-110,100)
communication.transmit()



