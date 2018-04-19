import cv2
import numpy as np
import time
import math



cap = cv2.VideoCapture(0)
#upperOrange = np.array([10,255,255])
#lowerOrange = np.array([0,150,150])
#upperRed = np.array([30,255,255])
#lowerRed = np.array([0,50,50])
upperGreen = np.array([70,255,255])
lowerGreen = np.array([35,50,50])
upperBlue = np.array([140,255,255])
lowerBlue = np.array([80,50,50])
cnts = []

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
    #print(newx, newy)
    if theta < 0:
        sign = 0
        thetaMag = abs(theta)
    if theta >= 0:
        sign = 1
        thetaMag = abs(theta)
    return (r, sign, thetaMag)

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

while(1):
    ret, img = cap.read()
    k = cv2.waitKey(30) & 0xff
    frame = cv2.resize(img, (0,0), fx=0.25, fy=0.25)
    if k == 27:
        break
    start = time.time()
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    #red = cv2.inRange(hsv, lowerRed, upperRed)
    kernel = np.ones((3,3), np.uint8)
    green = cv2.inRange(hsv, lowerGreen, upperGreen)
    blue = cv2.inRange(hsv, lowerBlue, upperBlue)
    green = cv2.erode(green, kernel, iterations = 1)
    blue = cv2.erode(blue, kernel, iterations = 1)

    #consolidated = cv2.addWeighted(red, 1.0, green, 0.5,0)
    consolidated = cv2.bitwise_xor(green,blue)
    cv2.imshow('frame',consolidated)
    _, cnts,    _ = cv2.findContours(consolidated, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    if len(cnts) > 0:
        # find the largest contour in the mask, then use
        # it to compute the minimum enclosing circle and
        # centroid
        for i in range(len(cnts)):
            c = cnts[i]
            ((x, y), radius) = cv2.minEnclosingCircle(c)
            M = cv2.moments(c)
            cv2.drawContours(frame, c, -1, (0,255,0), 3)
            if M["m00"] != 0:
                center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
                #print(center[1], center[0])

        # only proceed if the radius meets a minimum size
                if radius > 5:
        # draw the circle and centroid on the frame,
            # then update the list of tracked points
                    print(frame[y][x])

                    cv2.circle(frame, (int(x), int(y)), int(radius),
                    (0, 255, 255), 2)
                    cv2.circle(frame, center, 5, (0, 0, 255), -1)

    end = time.time()
    print(end-start)
    cv2.imshow('cicles',frame)



cap.release()
cv2.destroyAllWindows()
cv2.waitKey(1)
