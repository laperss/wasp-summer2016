#include "autopilot/ControlNode.h"
//#include "autopilot/DroneController.h"
#include "ros/ros.h"
#include "ros/package.h"
#include "boost/thread.hpp"
#include <signal.h>
//#include <time>
#include <pthread.h> // for sleep

unsigned int ros_header_timestamp_base = 0;

void takeoff(ros::Publisher pub)
{

    std_msgs::Empty msg;
    ROS_INFO("Drone is taking off");
    for(int i=0;i<=5;i++)
	pub.publish(msg);
}

void land(ros::Publisher pub)
{

    std_msgs::Empty msg;
    ROS_INFO("Drone is supposed to land now");
    for(int i=0;i<=5;i++)
	pub.publish(msg);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "run_drone");
    ROS_INFO("STARTED WASP SUMMER PROJECT ARDRONE");
    ros::NodeHandle n;
    sleep(1);
    ros::Publisher takeoff_pub = n.advertise<std_msgs::Empty>("/ardrone/takeoff", 10);
    sleep(1);
    ros::Publisher land_pub = n.advertise<std_msgs::Empty>("/ardrone/land", 10);
    sleep(1);    

    takeoff(takeoff_pub);
   
    
    sleep(1);

    land(land_pub);

    return 1;
    
}
