import cv2
import numpy as np
from picamera import PiCamera
from picamera.array import PiRGBArray
import time

camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 12
rawCapture = PiRGBArray(camera, size=(640, 480))
time.sleep(3)

def nothing():
    pass
#assign strings for ease of coding

hh='Hue High'
hl='Hue Low'
sh='Saturation High'
sl='Saturation Low'
vh='Value High'
vl='Value Low'
wnd = 'Colorbars'
cv2.namedWindow(wnd)
print ('done1')
#Begin Creating trackbars for each
cv2.createTrackbar(hl, wnd,0,179,nothing)
cv2.createTrackbar(hh, wnd,0,179,nothing)
cv2.createTrackbar(sl, wnd,0,255,nothing)
cv2.createTrackbar(sh, wnd,0,255,nothing)
cv2.createTrackbar(vl, wnd,0,255,nothing)
cv2.createTrackbar(vh, wnd,0,255,nothing)
print ('done')

cv2.namedWindow("Image")
cv2.namedWindow("Filtered")


for frames in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
    frame = frames.array
    hsv=cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    #read trackbar positions for each trackbar
    hul=cv2.getTrackbarPos(hl, wnd)
    huh=cv2.getTrackbarPos(hh, wnd)
    sal=cv2.getTrackbarPos(sl, wnd)
    sah=cv2.getTrackbarPos(sh, wnd)
    val=cv2.getTrackbarPos(vl, wnd)
    vah=cv2.getTrackbarPos(vh, wnd)
 
    #make array for final values
    HSVLOW=np.array([hul,sal,val])
    HSVHIGH=np.array([huh,sah,vah])
 
    #create a mask for that range
    mask = cv2.inRange(hsv,HSVLOW, HSVHIGH)
    cv2.imshow("Filtered", mask)
    cv2.imshow("Image", frame)
    k = cv2.waitKey(1)
    if k == 'q':
        break
    rawCapture.truncate(0)   

cv2.destroyAllWindows()
