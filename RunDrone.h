#ifndef _RUN_DRONE
#define _RUN_DRONE
#include "../autopilot/ControlNode.h"
#include "ros/ros.h"
#include "ros/package.h"
#include <ardrone_autonomy/Navdata.h>
#include "boost/thread.hpp"
#include <signal.h>
#include "ArdroneNavdata.h"
#include <sstream>
#include <pthread.h> // for sleep



void nav_callback(const ardrone_autonomy::Navdata::ConstPtr& msg)
{
    std::cout << "Navdata:" << msg->batteryPercent << "\n";
}

/*void vid_callback(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO("Video:: [%s]", msg->data.c_str());
}

void vel_callback(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO("Velocity:: [%s]", msg->data.c_str());
    }*/


void sendControlToDrone(ros::Publisher pub, ControlCommand cmd)
{
    ROS_INFO("Command sent");
    geometry_msgs::Twist cmdT;
    cmdT.angular.z = -cmd.yaw;
    cmdT.linear.z = cmd.gaz;
    cmdT.linear.x = -cmd.pitch;
    cmdT.linear.y = -cmd.roll;
    cmdT.angular.x = cmdT.angular.y = 0.1;
    //if(isControlling)
    //{
    for(int i=0;i<=1000;i++)
	pub.publish(cmdT);
    //lastSentControl = cmd;
    //}

    //lastControlSentMS = getMS(ros::Time::now());
}






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

void reset(ros::Publisher pub)
{

    std_msgs::Empty msg;
    ROS_INFO("Toggle reset");
    for(int i=0;i<=5;i++)
	pub.publish(msg);
}


void Control_Thread()
{
    //bool isControlling = true;
    
    ROS_INFO("STARTED WASP SUMMER PROJECT ARDRONE");

    ros::NodeHandle n;
    // sleep(1);
    // ros::Publisher takeoff_pub = n.advertise<std_msgs::Empty>("/ardrone/takeoff", 1000);
    // sleep(1);
    // ros::Publisher command_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
    // sleep(1);
    // ros::Publisher land_pub = n.advertise<std_msgs::Empty>("/ardrone/land", 1000);
    // sleep(1);    
    // ros::Publisher reset_pub = n.advertise<std_msgs::Empty>("/ardrone/reset", 1000);
    // sleep(1);


    ros::Subscriber navdata_sub = n.subscribe<ardrone_autonomy::Navdata>("/ardrone/navdata",1000,nav_callback);
    // ros::Subscriber vel_sub = n.subscribe("/ardrone/",1000,vel_callback);
    // ros::Subscriber vid_sub =  n.subscribe("/ardrone/navdata",1000,vid_callback);



    // takeoff(takeoff_pub);
    // sleep(5);
    // ControlCommand control;
    // control.gaz = control.pitch = control.roll = 0;
    // control.yaw=0.9;
    // sendControlToDrone(command_pub, control);
    ros::spin();


    // sleep(5);
    // land(land_pub);

    
}
#endif
