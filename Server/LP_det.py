import cv2
from matplotlib import pyplot as plt
import numpy as np
import imutils
import easyocr
import re


def has_at_least_three_numbers(input_string):
    # Use regular expression to find all digits in the string
    digits = re.findall(r'\d', input_string)

    # Check if the number of digits found is greater than or equal to 3
    return len(digits) >= 3


def extract_digits(input_string):

    # Use regular expression to find all digits in the string
    digits = re.findall(r'\d', input_string)

    # Join the digits into a single string
    digit_string = ''.join(digits)

    return digit_string


def lcp_detector(input_image):
    image = np.asarray(bytearray(input_image))
    image = cv2.imdecode(np.fromstring(image, dtype=np.uint8), cv2.IMREAD_UNCHANGED) 
    gray=cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)
    # plt.imshow(cv2.cvtColor(gray,cv2.COLOR_BGR2RGB))
    # plt.show()
    bfilter=cv2.bilateralFilter(gray,11,17,17)
    blurred = cv2.GaussianBlur(bfilter, (5, 5), 0)
    edged=cv2.Canny(blurred,30,200)
    # plt.imshow(cv2.cvtColor(edged,cv2.COLOR_BGR2RGB))
    # plt.show()
    keypoints=cv2.findContours(edged.copy(),cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    contours=imutils.grab_contours(keypoints)
    contours=sorted(contours,key=cv2.contourArea,reverse=True)
    location=None
    for contour in contours:
        #print("hello there")
        approx=cv2.approxPolyDP(contour,10,True)#the meaning of the 10 is how much we make broken lines as whole line(approximation)
        if(len(approx))>=3:
            location=approx
            #print(location)
            mask=np.zeros(gray.shape,np.uint8)
            new_image=cv2.drawContours(mask,[location],0,255,-1)#means the scale in which thw image is and how much to add in each side
            new_image=cv2.bitwise_and(new_image,new_image,mask=mask)#makes the other parts black
            # plt.imshow(cv2.cvtColor(new_image,cv2.COLOR_BGR2RGB))
            # plt.show()
            (x,y)=np.where(mask==255)
            (x1,y1)=(np.min(x),np.min(y))
            (x2,y2)=(np.max(x),np.max(y))
            cropped_image=gray[x1:x2,y1:y2]
            # plt.imshow(cv2.cvtColor(cropped_image,cv2.COLOR_BGR2RGB))
            # plt.show()
            reader=easyocr.Reader(['en'], gpu=False, verbose=False)
            result=reader.readtext(cropped_image,allowlist='0123456789')
            # print(result)
            if result!=[] and len(result[0][-2].strip()) >= 3 and has_at_least_three_numbers(result[0][-2]):
                    return extract_digits(result[0][-2])
    
    return "couldn't recognize"
