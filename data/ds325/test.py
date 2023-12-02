import cv2 #Import openCV
import sys #import Sys. Sys will be used for reading from the command line. We give Image name parameter with extension when we will run python script

#Read the image. The first Command line argument is the image
image = cv2.imread("fast_circles/000000_depth.tiff") #The function to read from an image into OpenCv is imread()

print(image.shape)

#imshow() is the function that displays the image on the screen.
#The first value is the title of the window, the second is the image file we have previously read.
cv2.imshow("OpenCV Image Reading", image)

cv2.waitKey(0) #is required so that the image doesn’t close immediately. It will 