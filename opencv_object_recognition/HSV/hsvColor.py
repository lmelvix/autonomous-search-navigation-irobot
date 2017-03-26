import cv2,cv
import numpy as np

greencounter=0
orangecounter=0
bluecounter=0
wrongcounter=0

flag=0
prevflag=0

lower_green = np.array([36,92,100]) # 1
upper_green = np.array([57,255,255]) # 11
lower_orange = np.array([7,58,128]) # 1
upper_orange = np.array([24,255,255]) # 11
lower_blue = np.array([100,150,0])
upper_blue = np.array([140,255,255])

prevcxb=0
prevcyb=0

cap = cv2.VideoCapture(0)
while(cap.isOpened()):
    rval, frame = cap.read()
    frame = cv2.flip(frame, 1)
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    bmask = cv2.inRange(hsv, lower_green, upper_green)
    bmask = cv2.erode(bmask, cv2.getStructuringElement(cv2.MORPH_ERODE, (3,3) ))
    bmask = cv2.dilate(bmask, cv2.getStructuringElement(cv2.MORPH_DILATE, (3,3) ))
    bmoments=cv2.moments(bmask, False)

    if(bmoments['m00'] >0):
        cxb = bmoments['m10']/bmoments['m00']
        cyb = bmoments['m01']/bmoments['m00']
    else:
        cxb=0
        cyb=0
#    if (abs(cxb-prevcxb)<5 and abs(cyb-prevcyb)<5 and cxb!=0 and cyb!=0):
    cv2.circle(frame, (int(cxb),int(cyb)),10,(0,0,255),-1)
    cv2.imshow("Frame",frame)
    cv2.waitKey(1)
