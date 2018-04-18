import cv2
import time
from picamera import PiCamera
from picamera.array import PiRGBArray
#from convexHull import *

camera = PiCamera()
camera.resolution = (640, 480)
#camera.shutter_speed = 1
camera.framerate = 90
rawCapture = PiRGBArray(camera, size=(640, 480))

cnt = 0
t0 = time.time()
t2 = 0
for f in camera.capture_continuous(rawCapture,format="bgr", use_video_port=True):
    t1 = time.time()
    print(t1-t2)	
    image = f.array
    #cv2.imshow("image", image)
    #cv2.waitKey(1)
    #thresholdingForOrange(image)
    rawCapture.truncate(0)
    
    cnt = cnt+1
    if (time.time() - t0 >1.0):
       t0=time.time()
       print (cnt)
       cnt = 0	
    t2 = time.time()
