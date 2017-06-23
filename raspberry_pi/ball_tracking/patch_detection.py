import cv2
import numpy as np
from picamera.array import PiRGBArray
from picamera import PiCamera
import math

blueLower = np.array([50,0,0])
blueUpper = np.array([70,255,255])
redLower = np.array([])
redUpper = np.array([])
greenLower = np.array([])
greenUpper = np.array([])
purpleLower = np.array([])
purpleUpper = np.array([])
kernel = np.ones((3,3), np.uint8)

def dist(point1, point2):
    return math.sqrt((point1[0]-point2[0])**2 + (point1[1]-point2[1])**2)

def getCentroid(contour):
    M = cv2.moments(contour)
    cX = int(M["m10"] / M["m00"]) 
    cY = int(M["m01"] / M["m00"])
    return (cX, cY)

def blobbify(Image, iter=1, minDist=10):
    Image = cv2.erode(Image, kernel, iterations=iter)
    Image = cv2.dilate(Image, kernel, iterations=iter)
    
    im2, contours, hierarchy = cv2.findContours(Image, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
    contours.sort(key=lambda contour: cv2.contourArea(contour))
    
    blobs = []
    for contour in contours:
        if len(blobs) == 0:
            blobs.append((getCentroid(contour),cv2.contourArea(contour)))
        else:
            found_blob = False
            (cX, cY) = getCentroid(contour)
            area = cv2.contourArea(contour)
            
            for blob in blobs:
                (bX, bY) = blob[0]
                if dist((bX, bY), (cX, cY)) < minDist: # dist is a function to be coded later
                    # combine blob
                    nX = (bX*blob[1] + cX*area)/(blob[1] + area)
                    nY = (bY*blob[1] + cY*area)/(blob[1] + area)
                    blob = ((nX, nY), blob[1] + area)
                    found_blob = True
                    break
            if not found_blob:
                blobs.append((getCentroid(contour),cv2.contourArea(contour)))

    return blobs

def identifyPatches(hsvImage):
    blue_img = cv2.inRange(hsvImage, blueLower, blueUpper)
    '''red_img = cv2.inRange(hsvImage, redLower, redUpper)
    green_img = cv2.inRange(hsvImage, greenLower, greenUpper)
    purple_img = cv2.inRange(hsvImage, purpleLower, purpleUpper)'''
    
    blue_blobs = blobbify(blue_img)
    print (blue_blobs)
    '''red_blobs = blobbify(red_img)
    green_blobs = blobbify(green_img)
    purple_blobs = blobbify(purple_img)'''
    