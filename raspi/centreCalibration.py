
import cv2
import numpy as np
import time

global ix,iy
ix = 0
iy = 0
def mouse_track(event,x,y,flags,param):
    if event == cv2.EVENT_LBUTTONDOWN:
        ix,iy = x,y
        print(ix,iy)
        print
        color = np.uint8([[[frame[iy][ix][0], frame[iy][ix][1], frame[iy][ix][2]]]])
        hsv_green = cv2.cvtColor(color,cv2.COLOR_BGR2HSV)
        print hsv_green


cap = cv2.VideoCapture(0)
while(1):
    ret, img = cap.read()
    k = cv2.waitKey(30) & 0xff
    frame = cv2.resize(img, (0,0), fx=0.25, fy=0.25)
    if k == 27:
        break
    color = cv2.setMouseCallback('frame',mouse_track)
    cv2.imshow('frame',frame)




cap.release()
cv2.destroyAllWindows()
cv2.waitKey(1)

c = np.array([[154,14], [206,39],[225,102],[192,133],[145,135],[104,107],[115,60]])
((x,y),radius) = cv2.minEnclosingCircle(c)
print(x,y,radius)
