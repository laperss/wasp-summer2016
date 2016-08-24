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
#include <tum_ardrone/filter_state.h>
#include <pthread.h> // for sleep

void nav_callback(const ardrone_autonomy::Navdata::ConstPtr& msg)
{
    std::cout << "nav callback\n";
    ardrone_navdata.Update_navdata(msg);
    return;
}

void pose_callback(const tum_ardrone::filter_state::ConstPtr& msg)
{
    std::cout << "pose callback\n";
    ardrone_navdata.Update_stateEstimation(msg);
    return;
}

void Subscribe_Thread()
{
    ros::NodeHandle n;
    // Subscribe to navdata
    //ros::Subscriber navdata_sub = n.subscribe<ardrone_autonomy::Navdata>("/ardrone/navdata",1000,nav_callback);

    // Subscribe to state estimation
    ros::Subscriber predictedPose_sub = n.subscribe<tum_ardrone::filter_state>("/ardrone/predictedPose",1000,pose_callback);

    ros::spin();
    return;
	
}

#endif





