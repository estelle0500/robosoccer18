import cv2
import numpy as np
import time
import math
import serial
import RPi.GPIO as GPIO
import time
from picamera.array import PiRGBArray
from picamera import PiCamera
from SerialClass.py import serialLink


camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(640, 480))
time.sleep(0.1)
#ser=serial.Serial("/dev/ttyACM0",9600)  #change ACM number as found from ls /dev/tty/ACM*
#ser.flushInput()

end1 = 0
end2 = 0
end3 = 0
end = 0

serialComm = serialLink()
#upperOrange = np.array([10,255,255])
#lowerOrange = np.array([0,150,150])
upperRed = np.array([20,255,200])
lowerRed = np.array([0,50,50])
upperGreen = np.array([60,140,200])
lowerGreen = np.array([45,50,50])
upperBlue = np.array([120,255,200])
lowerBlue = np.array([90,50,50])
cnts = []
centerpoint = [240,240]


while(1):
    for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
  # grab the raw NumPy array representing the image, then initialize the timestamp
  # and occupied/unoccupied text
        img = frame.array
        rawCapture.truncate(0)
        image = img[0:480, 80:560]
        k = cv2.waitKey(30) & 0xff
        frame = image
        if k == 27:
            break
        start = time.time()
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        red = cv2.inRange(hsv, lowerRed, upperRed)
        green = cv2.inRange(hsv, lowerGreen, upperGreen)
        blue = cv2.inRange(hsv, lowerBlue, upperBlue)
        consolidated = cv2.addWeighted(red, 1.0, green, 0.5,0)
        consolidated = cv2.addWeighted(consolidated, 0.5, blue , 1,0)
        _, cnts,    _ = cv2.findContours(consolidated, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        if len(cnts) > 0:
            # find the largest contour in the mask, then use
            # it to compute the minimum enclosing circle and
            # centroid
            for i in range(len(cnts)):
                c = cnts[i]
                ((x, y), radius) = cv2.minEnclosingCircle(c)
                M = cv2.moments(c)
                if M["m00"] != 0:
                    center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

            # only proceed if the radius meets a minimum size
                    if radius > 3:
            # draw the circle and centroid on the frame,
                # then update the list of tracked points
                        cv2.circle(frame, (int(x), int(y)), int(radius),
                        (0, 255, 255), 2)
                        cv2.circle(frame, center, 5, (0, 0, 255), -1)
                        print(img[x][y])
                    #polar = (interpolate(polar[0]),polar[1])
        end = time.time()
        print(end-start)
        cv2.imshow('frame',consolidated)


cap.release()
cv2.destroyAllWindows()
cv2.waitKey(1)
