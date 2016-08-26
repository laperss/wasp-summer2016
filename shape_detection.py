#!/usr/bin/env python
'''
This source will act as support to finish your project and does not follow best
coding practices.
'''
#Import Python Packages, ROS messages
from __future__ import print_function
from __future__ import division
import roslib
import sys
import rospy
import cv2
from std_msgs.msg import String
from sensor_msgs.msg import CompressedImage
from cv_bridge import CvBridge, CvBridgeError
import numpy as np
import time
from geometry_msgs.msg import PointStamped
from sensor_msgs.msg import Image
#import the custom message we created to store objects
from wasp_custom_msgs.msg import object_loc
import tf
from math import hypot

#Define Constants

#Focal Length of the Asus Prime sensor camera
focal_leng = 570.34222

#This may change during the competetion, need to be calibrated
square_side_lenth = 0.115 #in mts
class struct:
    def __init__(self):
        self.color = []
        self.shape = []

store = struct()

#global store

#This function finds the lengths of all the sides and estimates the longest.
def Longest_Length(approxcontour):
	#add the first element in the end to complete the loop
	approxcontour = np.concatenate((approxcontour,[approxcontour[0]]))
	#The below lines find the length between two adjacent points
	#and append them in  an array
	ptdiff = lambda (p1,p2): (p1[0]-p2[0], p1[1]-p2[1])
	diffs = map(ptdiff, zip(approxcontour,approxcontour[1:]))
	dist = []
	for d in diffs:
		dist.append(hypot(*d))
	#find maximum of lenghts found
	LongestSide = max(dist)
	return LongestSide

def find_color(hsv, lower, upper):
	found = 0
	# Threshold the HSV image to get only single color portions
	mask2 = cv2.inRange(hsv, lower, upper)
	#Find contours(borders) for the shapes in the image
	contours, hierarchy = cv2.findContours(mask2,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)

	for x in range (len(contours)):
		contourarea = cv2.contourArea(contours[x]) #get area of contour
		if (contourarea > 600 and contourarea > found): #Discard contours with a small area as this may just be noise
			found = contourarea
			return found
	return found

#This is the main class for object detection, it has some initializations about nodes
#Call back functions etc
class object_detection:
	def __init__(self):
		#Create Rospy Publisher and subscriber
		self.object_location_pub = rospy.Publisher("/object_location", object_loc, queue_size =1)
		#original images is huge and creates lot of latency, therefore subscribe to compressed image

		self.image_sub = rospy.Subscriber("/camera/rgb/image_raw/compressed",CompressedImage, self.callback)
		#self.image_sub = rospy.Subscriber("/ardrone/image_raw",Image,self.callback)
		#Cv Bridge is used to convert images from ROS messages to numpy array for openCV and vice versa
		self.bridge = CvBridge()
		#Obejct to transform listener which will be used to transform the points from one coordinate system to other.
		self.tl = tf.TransformListener()

	#Callback function for subscribed image
	def callback(self,data):
		#The below two functions conver the compressed image to opencv Image
		#'''
		np_arr = np.fromstring(data.data, np.uint8)
		cv_image = cv2.imdecode(np_arr, cv2.CV_LOAD_IMAGE_COLOR)
		#'''
		#cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
		#Create copy of captured image
		img_cpy = cv_image.copy()
		#Color to HSV and Gray Scale conversion
		hsv = cv2.cvtColor(cv_image, cv2.COLOR_BGR2HSV)
		#gray = cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)

		#Red_Thresholds
		lower_red1 = np.array([0, 100, 100])
		upper_red1 = np.array([10, 255,255])
		lower_red2 = np.array([160,100,100])
		upper_red2 = np.array([179,255,255])
		#Blue Thresholds
		lower_blue = np.array([104,110,110])
		upper_blue = np.array([143,255,255])
		#Green Thresholds
		lower_green = np.array([60,60,46])
		upper_green = np.array([97,255,255])
		

		# Threshold the HSV image to get only single color portions
		#mask2 = cv2.inRange(hsv, lower_blue, upper_blue)
		
		area= 0
		area_tmp = 0
		color = 0
		for x in range(1, 4):

			if x == 1:
				area_tmp = find_color(hsv, lower_red2, upper_red2)
				if area_tmp > 600:
					color = 1
					area = area_tmp
			elif x == 2:
				area_tmp = find_color(hsv, lower_green, upper_green)
				if area_tmp > area:
					color = 2
					area = area_tmp
			else:
				area_tmp = find_color(hsv, lower_blue, upper_blue)
				if area_tmp > area:
					color = 3
					area = area_tmp
		if color == 1:
			# Threshold the HSV image to get only single color portions
			mask2 = cv2.inRange(hsv, lower_red2, upper_red2)
		elif color == 2:
			# Threshold the HSV image to get only single color portions
			mask2 = cv2.inRange(hsv, lower_green, upper_green)
		elif color == 3:
			# Threshold the HSV image to get only single color portions
			mask2 = cv2.inRange(hsv, lower_blue, upper_blue)
		else:
			mask2 = cv2.inRange(hsv, lower_red1, upper_red1)
				
			

		#Find contours(borders) for the shapes in the image
		#NOTE if you get following error:
		# contours, hierarchy = cv2.findContours(mask2,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
		# ValueError: need more than two values to unpack
		# change following line to:
		# contours, hierarchy = cv2.findContours(mask2,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
		contours, hierarchy = cv2.findContours(mask2,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
		shape = 0
		#Pass through each contour and check if it has required properties to classify into required object
		for x in range (len(contours)):
			contourarea = cv2.contourArea(contours[x]) #get area of contour
			if contourarea > 600: #Discard contours with a small area as this may just be noise
				#The below 2 functions help you to approximate the contour to a nearest polygon
				arclength = cv2.arcLength(contours[x], True)
				approxcontour = cv2.approxPolyDP(contours[x], 0.02 * arclength, True)
				#Find the coordinates of the polygon with respect to he camera frame in pixels
				rect_cordi = cv2.minAreaRect(contours[x])
				obj_x = int(rect_cordi[0][0])
				obj_y = int(rect_cordi[0][1])

				#Check for Square
				#print ('Length ', len(approxcontour))
				if len(approxcontour) == 4:
					#print ('Length ', len(approxcontour))
					cv2.drawContours(cv_image,[approxcontour],0,(0,255,255),2)
					approxcontour = approxcontour.reshape((4,2))
					LongestSide = Longest_Length(approxcontour)
					Distance = (focal_leng*square_side_lenth)/LongestSide #focal length x Actual Border width / size of Border in pixels		
					shape = 10
				#Check for triangle
				elif len(approxcontour) == 3:
					#print ('Length ', len(approxcontour))
					cv2.drawContours(cv_image,[approxcontour],0,(0,255,255),2)
					approxcontour = approxcontour.reshape((3,2))
					LongestSide = Longest_Length(approxcontour)
					Distance = (focal_leng*square_side_lenth)/LongestSide #focal length x Actual Border width / size of Border in pixels
					shape = 20
				#Check for Start/Circle
				elif len(approxcontour) == 8:
					#print ('Length ', len(approxcontour))
					cv2.drawContours(cv_image,[approxcontour],0,(0,255,255),2)
					approxcontour = approxcontour.reshape((8,2))
					LongestSide = Longest_Length(approxcontour)
					Distance = (focal_leng*square_side_lenth)/LongestSide #focal length x Actual Border width / size of Border in pixels
					shape = 30
				#Move to next Contour
				
				else :
					continue

				#Calculate Cordinates wrt to Camera, convert to Map
				#Coordinates and publish message for storing
				#319.5, 239.5 = image centre
				obj_cam_x = ((obj_x - 319.5)*Distance)/focal_leng
				obj_cam_y = ((obj_y - 239.5)*Distance)/focal_leng

				#convert the x,y in camera frame to a geometric stamped point
				P = PointStamped()
				P.header.stamp = rospy.Time.now() - rospy.Time(1)
				#print ('time: ', data.header.stamp)
				P.header.frame_id = 'camera_rgb_optical_frame'
				P.point.x = obj_cam_x
				P.point.y = obj_cam_y
				P.point.z = Distance
				#try:
					#Transform Point into map coordinates
				t = self.tl.getLatestCommonTime("/base_link", "/map")
				tb_pos, tb_quat = self.tl.lookupTransform("/base_link", "/map", t)
				print("P:", P)
				trans_pt = self.tl.transformPoint("/map", P)
					
				print("trans_ot:", trans_pt)
				#except (tf.ExtrapolationException):
				#	continue

		

				#fill in the publisher object to publish
				obj_info_pub = object_loc()
				obj_info_pub.ID = color + shape + 1000 #ID need to be changed
				obj_info_pub.point.x = trans_pt.point.x
				obj_info_pub.point.y = trans_pt.point.y
				obj_info_pub.point.z = trans_pt.point.z
				#obj_info_pub.point.x = P.point.x
				#obj_info_pub.point.y = P.point.y
				#obj_info_pub.point.z = P.point.z

				#publish the message
				self.object_location_pub.publish(obj_info_pub)


		#Display the captured image
		cv2.imshow("Image",cv_image)
		#cv2.imshow("HSV", hsv)
		cv2.waitKey(1)


#Main function for the node
def main(args):
	rospy.init_node('object_detection', anonymous = False)
	ic = object_detection()
	try:
		rospy.spin()
	except KeyboardInterrupt:
		print('Shutting Down object_detection Node')
		cv2.destroyAllWindows()

if __name__ == '__main__':
	main(sys.argv)
