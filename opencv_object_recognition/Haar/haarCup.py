import cv2
import numpy as np
import math

cup = cv2.CascadeClassifier('cup.xml')

cap = cv2.VideoCapture(1)

while(cap.isOpened()):
    ret, img = cap.read()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    handCup = cup.detectMultiScale(gray, 1.3, 5)
    for (x,y,w,h) in handCup:
        cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)
        print "cup    ",x+(w/2),"    ",y+(h/2)


    cv2.imshow('img',img)
    k = cv2.waitKey(1)
    if k == 27:
        break

