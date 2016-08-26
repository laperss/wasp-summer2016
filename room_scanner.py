#!/usr/bin/env python
'''
This source will act as support to finish your project and does not follow best
coding practices.
'''
#Import Python Packages, ROS messages

import rospy
from geometry_msgs.msg import PoseArray
from geometry_msgs.msg import PoseStamped
from actionlib_msgs.msg import GoalStatusArray
from std_msgs.msg import Int16
import time
#create an object of type Pose Array message, this will hold the list of points
#that are published from tb_rviz_interaction
goal_list = PoseArray()

#Initialize variable for goal index and goal flag , this is used to hold index
#published to tb_rviz_interaction
goal_index = 0
goal_flag = True
goal_reached = goal_index + 1

#Initialize an object of type Pose Stamped message, this is used to publish
#message to Robot
goto_to_publish = PoseStamped()
goto_to_publish.header.frame_id = "/map"
goto_to_publish.pose.orientation.z = 1

def goto(x, y, w):
	#goto_to_publish.header.frame_id = "/map"
	goto_to_publish.pose.position.x = x
	goto_to_publish.pose.position.y = y
	#goto_to_publish.pose.orientation.z = 1
	goto_to_publish.pose.orientation.w = w

	pub2.publish(goto_to_publish)
	clock = 0
	global goal_reached
	goal_reached = goal_index + 1
	print("reached = ") 
	print(goal_reached)
	print("index = ")
	print(goal_index)
	while goal_reached != goal_index:
		time.sleep(1)
		clock = clock + 1
			
		if clock == 15:
			break
	print("")
	print("reached = ") 
	print(goal_reached)
	print("index = ")
	print(goal_index)
	print("")
	goal_reached = goal_reached + 1
	
		

#This callback is called everytime this node receives the status message of Robot
#The Status message is published frequently from node, always has the last goal
# status if no new goals are published.
#Goal status information : http://docs.ros.org/api/actionlib_msgs/html/msg/GoalStatus.html
#status = 1 -> Active
#status = 4 -> failed
#status = 3 -> completed
def goal_status_callback(data):
	global goal_flag
	#print(data.status_list[0].status)
	#if data.status_list[0].status == 1:
	#	goal_flag = True

	if len(data.status_list)!=0:
		if(goal_flag == True and ((data.status_list[0].status == 4 ) or (data.status_list[0].status == 3))):
				#current accomplished goal
				global goal_index
				pub1.publish(goal_index)
				goal_index = goal_index + 1
				goal_flag = False
				print(data.status_list[0].status)


# Intializes everything
def start():
	# Create Global Publishers
	global pub1,pub2
	#Initialize current node with some name
	rospy.init_node('tb_path_publisher')
	#Assigin publisher that publishes the index of the goal just accomplished
	pub1 = rospy.Publisher('/goal_completed', Int16, queue_size=1)
	#Assign Publisher that publishes the goal to the robot to move
	pub2 = rospy.Publisher('/move_base_simple/goal', PoseStamped, queue_size=10)
	time.sleep(1)
	#subscribe to list of goals from rviz interaction package
	#rospy.Subscriber("/list_of_goals", PoseArray, new_goal_list_callback)
	#subscribe to goal status from mobile base
	rospy.Subscriber("/move_base/status", GoalStatusArray, goal_status_callback)
	#This keeps the function active till node are shurdown.

	goto(0, 0, 0)
	goto(1, 1, 1.57)
	goto(1, 1, 0.52)
	goto(1.25, 0.5, 0)
	goto(1.5, -0.5, 0)
	goto(2, -1, 0.52)
	goto(2, -2, 1.05)
	goto(2, -2, -0.52)
	goto(2, -2, 1.05)
	goto(2, -2, -2.09)
	goto(1, -2, -1.57)
	goto(1, -2, -2.09)
	goto(1 , -2, -2.62)
	goto(0, -2, -3.14)
	goto(-1, -2.5, -1.05)
	goto(-1,-1.5, -2.09)
	goto(-2.5, -0.5, -1.57)
	goto(-3.5, -3, -1.57)		
	goto(-3.5, -3, -2.62)
	goto(-3.5, -2, -2.62)
	goto(0, 0, 0)

	
	
	print("done")
	rospy.spin()

#Main function
if __name__ == '__main__':
	start()
