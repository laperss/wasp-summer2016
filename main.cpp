//main.cpp
//Oskar
#include <boost/thread.hpp>
#include "ros/ros.h"
#include "SubScriber.h"
#include "RunDrone.h" //here we need a Control_Thread here

unsigned int ros_header_timestamp_base = 0;


int main(int argc, char** argv) 
{
    ros::init(argc, argv, "run_drone");
    boost::thread thread_subscriber(Subscribe_Thread); 
    boost::thread thread_controller(Control_Thread); 
    thread_subscriber.join();
    thread_controller.join();
    return 0;
}
