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
goal = 'yellow' #or goal = "blue"
CenterList = []

serialComm = serialLink()

def transformCoordinates(x,y,centre):
    newx = x - centre[0]
    newy = centre[1] - y
    r = (newx**2 + newy**2)**0.5
    if newx == 0:
        newx = 1

    theta = math.atan2(newy,newx)*360/(2*3.14)
    theta = (theta - 45)%360
    if theta > 180:
        theta = theta - 360

    return (theta, r)

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
upperYellow = np.array([15,255,255])
lowerYellow = np.array([0,50,50])
cnts = []
centerpoint = [240,240]
while(1):
    for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
        serialComm.add(255)
        serialComm.add(255)
        serialComm.add(255)

  # grab the raw NumPy array representing the image, then initialize the timestamp
  # and occupied/unoccupied text
        img = frame.array
        rawCapture.truncate(0)
        image = img[0:480, 80:560]
        k = cv2.waitKey(30) & 0xff
        if k == 27:
            break
        start = time.time()
        hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
        end1 = time.time()
        #print(end1-start," hsv")
        if goal == 'yellow':
            mask = cv2.inRange(hsv, lowerYellow, upperYellow)
            kernel = np.ones((5,5), np.uint8)
            mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)

        if goal == 'blue':
            mask = cv2.inRange(hsv, lowerBlue, upperBlue)
            kernel = np.ones((5,5), np.uint8)
            mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)

        _, cnts,    _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        end2 = time.time()
        end = 0;
        #print(end2-end1," contour")
        if len(cnts) > 0:
            # find the largest contour in the mask, then use
            # it to compute the minimum enclosing circle and
            # centroid
            if len(cnts) > 1:

                for i in range(len(cnts)):
                    M = cv2.moments(cnts[i])
                    if M["m00"] != 0:
                        checkDistanceList = []
                        center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
                        center = transformCoordinates(center)
                        checkDistanceList.append(center)
                        completed = False

                        while completed == False:
                            length = len(checkDistanceList)
                            merged = False
                            tempGoalList = []
                            for i in range(len(checkDistanceList)-1):

                                distance = (checkDistanceList[i][1]**2 + checkDistanceList[i+1][1]**2 - 2*(checkDistanceList[i][1])*(checkDistanceList[i+1][1]*math.cos(abs(checkDistanceList[i][0]-checkDistanceList[i+1][0]))))**0.5
                                ref = 0.5 * 60

                                if distance < ref:
                                    checkDistanceList[i+1] = [((checkDistanceList[i][0] + checkDistanceList[i+1][0])/2),((checkDistanceList[i][1] + checkDistanceList[i+1][1])/2)]
                                    checkDistanceList[i] = [1000,1000]

                                    merged = True
                                for i in range(len(checkDistanceList)):
                                    if checkDistanceList[i] == [1000,1000]:
                                        tempGoalList.append(i)
                            tempGoalList = list(set(tempGoalList))

                            tempGoalList.sort(reverse=True)
                            for i in tempGoalList:
                                del checkDistanceList[i]
                            tempGoalList = []
                            if merged == True:
                                completed = False
                            else:
                                completed = True
                        for i in checkDistanceList:
                            CenterList.append(i)


            CenterList.sort(key=lambda x: x[1])
            goalPosition = CenterList[0]
            print(goalPosition)

            c = max(cnts, key=cv2.contourArea)
            M = cv2.moments(c)

            if M["m00"] != 0:
                center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
                end3 = time.time()
                #print(end3-end2, "centre")

            # only proceed if the radius meets a minimum size
            # draw the circle and centroid on the frame,
                # then update the list of tracked points
                cv2.circle(image, center, 5, (0, 0, 255), -1)
                polar = transformCoordinates(int(center[0]),int(center[1]),centerpoint)
                polar[1] = interpolate(polar[1])
                polar = (int(polar[0])),int(solar[1])
                end = time.time()
                print(polar)
                #print(end4-end3," polar")

                serialComm.addDetectedObject(5, polar[0],polar[1])
                serialComm.write(254)
                serialComm.write(254)
                serialComm.write(254)



        #print(end1-start,"hsv")
        #print(end2-end1,"contour")
        #print(end3-end2,"center")
        #print(end-end3,"polar")
        #print(end-start)
        cv2.imshow('frame',mask)


cap.release()
cv2.destroyAllWindows()
cv2.waitKey(1)
