import cv2
import numpy as np
from picamera.array import PiRGBArray
from picamera import PiCamera
import math
import time
from serialBallLocation import *
from patchDetection import *

def nothing():
    pass

def selectOrange(hsvImage):
    lowerOrange = np.array([0, 100, 150])
    upperOrange = np.array([15, 200, 255])
    
    cv2.namedWindow('Filter')
    mask = cv2.inRange(hsvImage, lowerOrange, upperOrange)    # erode and dilate to remove small noise
    edSize = characteristicSize
    kernel = np.ones((edSize, edSize), np.uint8)
    mask = cv2.erode(mask, kernel, iterations=1)
    mask = cv2.dilate(mask, kernel, iterations=1)
    cv2.imshow('Filter', mask)
    return mask

def displayAngle(angle, originalImage):
    font = cv2.FONT_HERSHEY_SIMPLEX
    cv2.putText(originalImage,str(angle),(20,475), font,0.8,(255,255,255),2,cv2.LINE_AA)
    return originalImage

def findBall (binarizedImage, originalImage):
    cv2.namedWindow('Image')
    im2, contours, hierarchy = cv2.findContours(binarizedImage, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
    # cv2.drawContours(originalImage, contours, -1, (0,255,0), 3)

    maxArea = -1
    for contour in contours:
        area = cv2.contourArea(contour)
        if (area > maxArea):
            maxArea = area
            maxContour = contour
    
    if maxArea < 5:
        return None
    #x,y,w,h = cv2.boundingRect(maxContour)
    #cv2.rectangle(originalImage, (x,y), (x+w,y+h), (0,255,0), 2)

    M = cv2.moments(maxContour)
    cX = int(M["m10"] / M["m00"]) 
    cY = int(M["m01"] / M["m00"])
    ballCentre = np.array([cX, cY])
    # print (cX, cY)
    # cv2.circle(originalImage, (cX,cY), 3, [255,0,0])
    centrePoint = (320,240)
    topPoint = (320,0)
    cv2.line(originalImage, (cX, cY), centrePoint, (255,0,0), 3)
    cv2.line(originalImage, centrePoint, topPoint, (255,0,0), 3)
    
    angle = math.atan2(cX-320, 240-cY)
    dispImage = displayAngle(angle, originalImage)
    cv2.namedWindow('Image')
    cv2.imshow('Image', dispImage)
    return angle

if __name__ == "__main__":
    camera = PiCamera()
    camera.resolution = (640, 480)
    camera.framerate = 12
    rawCapture = PiRGBArray(camera, size=(640, 480))
    time.sleep(3)
    characteristicSize = 3
    ser = initSerial()
    
    for frames in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
        # grab the raw NumPy array representing the image, then initialize the timestamp
        # and occupied/unoccupied text
        frame = frames.array
        hsvImage = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        identifyPatches(frame)
        
        binarizedImg = selectOrange(frame)
        angle = findBall(binarizedImg, frame)
        print (angle)
        if angle is not None:
            sendFloat(ser, angle)
        rawCapture.truncate(0)
        
        k = cv2.waitKey(0)
        if (k == 'q'):
            break
    
    cv2.destroyAllWindows()