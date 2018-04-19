import cv2
import numpy as np
import time
import math
from EstablishSerial import serialLink


serialComm = serialLink()
centerpoint = [228,238]

upperOrange = np.array([20,255,255])
lowerOrange = np.array([0,120,60])
upperOrange2 = np.array([179,255,255])
lowerOrange2 = np.array([160,120,60])

upperGreen = np.array([70,255,150])
lowerGreen = np.array([50,100,80])

dilateKernel = np.ones((3, 3), np.uint8)
erodeKernel = np.ones((5, 5), np.uint8)

imageProc_dubug = False

def thresholdingForOrange(rawImg):
    t_st = time.time()
    hsv = cv2.cvtColor(rawImg, cv2.COLOR_BGR2HSV)
    mask1 = cv2.inRange(hsv, lowerOrange, upperOrange)
    mask2 = cv2.inRange(hsv, lowerOrange2, upperOrange2)
    mask = cv2.bitwise_or(mask1,mask2)
    #mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, dilateKernel, iterations=1)
    mask = cv2.erode(mask, erodeKernel, iterations=1)
    greenMask = cv2.inRange(hsv, lowerGreen, upperGreen)
    greenMask = cv2.erode(greenMask, erodeKernel, iterations=1)
    #greenMask = cv2.morphologyEx(greenMask, cv2.MORPH_OPEN, dilateKernel, iterations=1)
    _, greenCnts, _ = cv2.findContours(greenMask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    hullMask = findConvexHull(rawImg,greenCnts)
    if hullMask == None:
        return None,None
    reducedMask = cv2.bitwise_and(mask,hullMask)
    _, cnts, _ = cv2.findContours(reducedMask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    t_ed = time.time()
    #print("tot")
    #print(t_ed-t_st)
    #t_st= time.time()
    if (imageProc_dubug):
        cv2.imshow("originalMask", mask)
        time.sleep(0.1)
        cv2.imshow("greenMask", greenMask)
        time.sleep(0.1)
        cv2.imshow("hullMask",hullMask)
        time.sleep(0.1)
        cv2.drawContours(rawImg,cnts,-1,(255,0,0))
        cv2.imshow("reducedMask",rawImg)
        #cv2.waitKey(10)
    sendLocation(cnts)
    return mask,cnts


def findConvexHull(image,contours):
    # join all the contours together and make them into a single point list
    time_st = time.time()
    if len(contours) == 0:
        return None
    fullCNT = np.vstack(contour for contour in contours if cv2.contourArea(contour)>100)
   # for contour in contours:
        #fullCNT = np.concatenate((fullCNT,contours),axis = 0)
        #for element in contour:
            #fullCNTs = fullCNTs.append([[[element[0][0],element[0][1]]]])
    #fullCNT = np.array(fullCNTs)
    #time_end = time.time()
    #print("join cnt")
    #print(time_end-time_st)
    #time_st = time.time()
    hull = cv2.convexHull(fullCNT)
    #time_end = time.time()
    #print("onvex hull")
    #print(time_end-time_st)
    
    height, width, depth = image.shape
    hullMask = np.zeros((height, width), np.uint8)
    cv2.fillPoly(hullMask, pts=[hull], color=(255, 255, 255))
    # cv2.imshow("hull", hullMask)
    # time.sleep(0.1)
    return hullMask


def transformCoordinates(x,y,centre):
    newx = x - centre[0]
    newy = centre[1] - y
    if abs(newx) <20:
        newx = 0
    r = (newx**2 + newy**2)**0.5
    if newx == 0:
        newx = 1

    theta = math.atan2(newy,newx)*360/(2*3.14)
    theta = (theta - 45)%360
    if theta > 180:
        theta = theta - 360


    return (theta, r)

def sendLocation(cnts):
    t00 = time.time()
    serialComm.add(255)
    serialComm.add(255)
    serialComm.add(255)
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
                # cv2.circle(image, (int(x), int(y)), int(radius),
                # (0, 255, 255), 2)
                # cv2.circle(image, center, 5, (0, 0, 255), -1)
                polar = transformCoordinates(int(x),int(y),centerpoint)
                #polar[1] = interpolate(polar[1])
                polar = (int(polar[0])),int(polar[1])
                print(polar)
                #print(end4-end3," polar")

                serialComm.addDetectedObject(5, polar[0],polar[1])
                serialComm.add(254)
                serialComm.add(254)
                serialComm.add(254)
                serialComm.transmit()
        #print(time.time()- t00)
