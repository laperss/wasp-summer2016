//Subscriber.cpp
//Oskar
/*
Creates a subscriber to the topic "ardrone/Navdata" and writes it to
the datamonitor ArdroneNavdata.h

 */


#ifndef _SUBSCRIBER
#define _SUBSCRIBER

#include "boost/thread.hpp"
#include "ArdroneNavdata.h"
#include <ardrone_autonomy/Navdata.h>

#include <pthread.h> // for sleep

void nav_callback(const ardrone_autonomy::Navdata::ConstPtr& msg)
{
    ardrone_navdata.Update(msg);
    return;
}



void Subscribe_Thread()
{
    ros::NodeHandle n;
    ros::Subscriber navdata_sub = n.subscribe<ardrone_autonomy::Navdata>("/ardrone/navdata",1000,nav_callback);

    ros::spin();
    return;
	
}

#endif





