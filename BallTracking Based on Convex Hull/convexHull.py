import cv2
import numpy as np
import time

upperOrange = np.array([20,255,255])
lowerOrange = np.array([0,90,40])
upperOrange2 = np.array([179,255,255])
lowerOrange2 = np.array([160,90,40])

upperGreen = np.array([70,255,255])
lowerGreen = np.array([50,40,40])

dilateKernel = np.ones((3, 3), np.uint8)
erodeKernel = np.ones((3, 3), np.uint8)

imageProc_dubug = True

def thresholdingForOrange(rawImg):
    hsv = cv2.cvtColor(rawImg, cv2.COLOR_BGR2HSV)
    mask1 = cv2.inRange(hsv, lowerOrange, upperOrange)
    mask2 = cv2.inRange(hsv, lowerOrange2, upperOrange2)
    mask = cv2.addWeighted(mask1, 1, mask2, 1, 0)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, dilateKernel, iterations=1)
    mask = cv2.erode(mask, erodeKernel, iterations=1)
    greenMask = cv2.inRange(hsv, lowerGreen, upperGreen)
    greenMask = cv2.erode(greenMask, erodeKernel, iterations=3)
    greenMask = cv2.morphologyEx(greenMask, cv2.MORPH_OPEN, dilateKernel, iterations=4)
    _, greenCnts, _ = cv2.findContours(greenMask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    hullMask = findConvexHull(rawImg,greenCnts)
    reducedMask = cv2.bitwise_and(mask,hullMask)
    _, cnts, _ = cv2.findContours(reducedMask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    if (imageProc_dubug):
        cv2.imshow("originalMask", mask)
        time.sleep(0.1)
        cv2.imshow("greenMask", greenMask)
        time.sleep(0.1)
        cv2.imshow("hullMask",hullMask)
        time.sleep(0.1)
        cv2.drawContours(rawImg,cnts,-1,(255,0,0))
        cv2.imshow("reducedMask",rawImg)
    return mask,cnts


def findConvexHull(image,contours):
    # join all the contours together and make them into a single point list
    fullCNTs = []
    for contour in contours:
        for element in contour:
            fullCNTs = fullCNTs + [[[element[0][0],element[0][1]]]]
    fullCNT = np.array(fullCNTs)
    hull = cv2.convexHull(fullCNT)
    height, width, depth = image.shape
    hullMask = np.zeros((height, width), np.uint8)
    cv2.fillPoly(hullMask, pts=[hull], color=(255, 255, 255))
    cv2.imshow("hull", hullMask)
    cv2.waitKey(0)
    return hullMask

