# import the necessary packages
import cv2
import numpy as np
import time
import math
import serial
import RPi.GPIO as GPIO
import time
from picamera.array import PiRGBArray
from picamera import PiCamera
from threading import Thread


class PiVideoStream:
    def __init__(self, resolution=(640, 480), framerate=32):
        # initialize the camera and stream
        self.camera = PiCamera()
        self.camera.resolution = (640, 480)
        self.camera.framerate = 32
        self.rawCapture = PiRGBArray(self.camera, size=(640, 480))
        self.stream = self.camera.capture_continuous(self.rawCapture,
                                                     format="bgr", use_video_port=True)
        time.sleep(0.1)
        self.newFrameAvailable = False
        
        self.camera.awb_mode = 'off'
        i = 5*1.4
        j = 9*1.4
        self.camera.awb_gains = (1.0+0.05*i,1.0+.05*j)


        # initialize the frame and the variable used to indicate
        # if the thread should be stopped
        self.frame = None
        self.stopped = False
        self.previousTime = 0
        self.currentTime = time.time()
        self.number = 0
        self.haveNewFrame = False
    def start(self):
        # start the thread to read frames from the video stream
        self.currentThread = Thread(target=self.update, args=())
        self.currentThread.start()
        return self

    def update(self):
        # keep looping infinitely until the thread is stopped
        for f in self.stream:
            if self.stopped:
                self.stream.close()
                self.rawCapture.close()
                self.camera.close()
                return
            # grab the frame from the stream and clear the stream in
            # preparation for the next frame
            self.frame = f.array
            self.haveNewFrame = True
            self.rawCapture.truncate(0)
            # self.previousTime = self.currentTime
            #self.currentTime = time.time()
            #self.number +=1
            # if the thread indicator variable is set, stop the thread
            # and resource camera resources

    def read(self):
        # return the frame most recently read
        self.haveNewFrame = False
        return self.frame

    def stop(self):
        # indicate that the thread should be stopped
        self.stopped = True
        cv2.destroyAllWindows()
