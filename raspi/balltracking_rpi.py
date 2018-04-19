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
ser=serial.Serial("/dev/ttyACM0",9600)  #change ACM number as found from ls /dev/tty/ACM*
ser.flushInput()


def transformCoordinates(x,y,centre):
    newx = x - centre[0]
    newy = centre[1] - y
    r = (newx**2 + newy**2)**0.5
    theta = math.atan(abs(newy)/abs(newx))*360/(2*3.14)
    if newx > 0 and newy > 0:
        theta = 90 - theta
    if newx > 0 and newy < 0:
        theta = 90 + theta
    if newx < 0 and newy < 0:
        theta = -90 - theta
    if newx < 0 and newy > 0:
        theta = -90 + theta
    return (r, theta)

def interpolate(r):
    readings = [10,15,20,25,30,35,40,45]
    calibration = [20,50,70,85,95,100,103,104]
    if r > calibration[-1]:
        adjusted = readings[-1] + 1
        return adjusted
    if r < calibration[0]:
        adjusted = 10
        return adjusted
    for i in range(len(calibration)-1):
        if r > calibration[i]:
            adjusted = (r - calibration[i]) / (calibration[i+1] - calibration[i]) * 5 + (10 + 5 * i)

    return adjusted

#upperOrange = np.array([10,255,255])
#lowerOrange = np.array([0,150,150])
upperOrange = np.array([10,255,166])
lowerOrange = np.array([0,70,70])
cnts = []
centerpoint = [235,200]
while(1):
    for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
	# grab the raw NumPy array representing the image, then initialize the timestamp
	# and occupied/unoccupied text
    	img = frame.array
    	rawCapture.truncate(0)
        image = cv2.resize(img, (0,0), fx=0.25, fy=0.25)
        k = cv2.waitKey(30) & 0xff
        if k == 27:
            break
        start = time.time()
        hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsv, lowerOrange, upperOrange)
        _, cnts,    _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        if len(cnts) > 0:
            # find the largest contour in the mask, then use
            # it to compute the minimum enclosing circle and
            # centroid
            c = max(cnts, key=cv2.contourArea)
            ((x, y), radius) = cv2.minEnclosingCircle(c)
            M = cv2.moments(c)
            if M["m00"] != 0:
                center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

            # only proceed if the radius meets a minimum size
                if radius > 3:
            # draw the circle and centroid on the frame,
                # then update the list of tracked points
                    cv2.circle(image, (int(x), int(y)), int(radius),
                    (0, 255, 255), 2)
                    cv2.circle(image, center, 5, (0, 0, 255), -1)
                    polar = transformCoordinates(x,y,centerpoint)
                    polar = (int(interpolate(polar[0])),int(polar[1]))
                    print(polar)
                    ser.write('254')
                    ser.write(polar[0]) #distance
                    ser.write(polar/abs(polar)) #1
                    ser.write(abs(polar)) #2
                    ser.write('255')
    end = time.time()
    print(end-start)
    cv2.imshow('frame',image)


cap.release()
cv2.destroyAllWindows()
cv2.waitKey(1)
