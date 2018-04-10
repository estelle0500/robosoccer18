import RPi.GPIO
import serial.tools.list_ports
import time
import serial

class serialLink(object):

    def __init__(self):
        TeensyFound, ser = initSerial()
        while (not TeensyFound):
            time.sleep(500)
            TeensyFound, ser = initSerial()
        self.ser = ser


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

    def handleSerialException():
        TeensyFound = false
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
                    TeensyFound = true
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



    def write(self, number):
        try:
            self.ser.write(str(bytearray([number])))
        except:
            handleSerialException()

    # def begin(self):
    #     try:
    #         self.write(str(bytearray([number])))
    #     except:
    #         handleSerialException()
    #
    # def end(self):
    #     self.write(str(bytearray([255])))
    #
    # def sendVal(self, serialport, val1, val2, val3):
    #     serialport.write(str(bytearray([val1, val2, val3])))

serialComm = serialLink()
while(1):
    if False:
        break
    else:
        serialComm.write(10)
        time.sleep(100)
