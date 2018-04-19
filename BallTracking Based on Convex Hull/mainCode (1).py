import cv2
import numpy as np
import time
import math
import serial
import RPi.GPIO as GPIO
import time
from picamera.array import PiRGBArray
from picamera import PiCamera
#from imageProc import *
#from FilterContour import *
from convexHull import *
from PiVideoStream import *

#setting up other classes

counter = 0
cycle_count = 0
t0 = time.time()
ted = 0

vs = PiVideoStream().start()
time.sleep(2.0)

# capture frames from the camera
while(1):
   # flush away the empty frames
    if (vs.haveNewFrame):
        cycle_count = cycle_count + 1
    image = vs.read()[0:480,60:540]
    if (counter < 10):
        counter = counter + 1
        print("flushing")
        continue
    tanother = time.time()
    #print("other process")
    #print(tanother - ted)
       
    tst = time.time()
    # show the frame
    '''
    cv2.imshow('Frame', image)
    k = cv2.waitKey(30) & 0xff
    if k == 27:
        break


    '''
    # time.sleep(0.1)
    mask, cnts = thresholdingForOrange(image)
    if mask == None:
        pass
    #sendLocation(cnts)

    else:
        if (time.time()-t0>1.0):
            print (cycle_count)
            t0 = time.time()
            cycle_count = 0
        #mask, cnts =thresholdingForOrange(image)
        #if len(cnts) > 0:
        #     print(selectBall(image,cnts))
        #else:
        #    print("no contour found")
        ted = time.time()
        #print(tst - ted)
        ted = time.time()
        tedd = time.time()
        #print("truncate")
        #print(tedd - ted)
