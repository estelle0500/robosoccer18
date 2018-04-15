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

# setting up the camera
camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(640, 480))
time.sleep(0.1)
camera.awb_mode = 'off'
i = 5*1.4
j = 9*1.4
camera.awb_gains = (1.0+0.05*i,1.0+.05*j)

#setting up other classes

counter = 0

# capture frames from the camera
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
   # flush away the empty frames
    if (counter < 10):
        counter = counter + 1
        rawCapture.truncate(0)
        continue
    # show the frame
    image = frame.array[0:480, 80:560]
    cv2.imshow('Frame', image)
    k = cv2.waitKey(30) & 0xff
    if k == 27:
        break

    time.sleep(0.1)
    thresholdingForOrange(image)
    
    #mask, cnts =thresholdingForOrange(image)
    #if len(cnts) > 0:
    #     print(selectBall(image,cnts))
    #else:
    #    print("no contour found")

    rawCapture.truncate(0)