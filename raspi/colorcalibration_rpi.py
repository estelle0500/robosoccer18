import cv2
import numpy as np
import time
import math
import serial
import RPi.GPIO as GPIO
import time
from picamera.array import PiRGBArray
from picamera import PiCamera

camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(640, 480))
time.sleep(0.1)


color = []
global ix,i
ix = 0
iy = 0
def mouse_track(event,x,y,flags,param):
    if event == cv2.EVENT_LBUTTONDOWN:
        ix,iy = x,y
        print(ix,iy)
        print(img[iy][ix])

for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
    image = frame.array
    rawCapture.truncate(0)
    k = cv2.waitKey(30) & 0xff
    img = cv2.resize(image, (0,0), fx=0.25, fy=0.25)
    if k == 27:
        break
    cv2.setMouseCallback('frame',mouse_track)
    cv2.imshow('frame',img)



cap.release()
cv2.destroyAllWindows()
cv2.waitKey(1)
