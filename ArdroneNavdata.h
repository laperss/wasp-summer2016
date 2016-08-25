//ArdroneNavdata.h
//Oskar
/*
Datamonitor to store the navdata from the ardrone. It stores the
navdata as is, i.e. as an "ardrone_autonomy::Navdata"
 */

/*	//commands for state_estimatin:
	x = state_estimation.x;
	y = state_estimation.y;
	z = state_estimation.z;
	roll = state_estimation.roll;
	pitch = state_estimation.pitch;
	yaw = state_estimation.yaw;
	dyaw = state_estimation.dyaw;
	state = state_estimation.droneState;
	batterylvl = state_estimation.batteryPercent;
*/

/*
	//commands for local_navdata:
	vx = local_navdata.vx;
	vy = local_navdata.vy;
	vz = local_navdata.vz;
	...
*/

#include <boost/thread.hpp>
#include <ardrone_autonomy/Navdata.h>
#include <dynamic_reconfigure/ConfigDescription.h>
#include <tum_ardrone/filter_state.h>
#include <wasp_custom_msgs/object_loc.h>

#ifndef _ARDRONE_NAVDATA
#define _ARDRONE_NAVDATA

class ArdroneNavdata {

private:

    boost::mutex mtx_;
    ardrone_autonomy::Navdata local_navdata;
    tum_ardrone::filter_state state_estimation;
    wasp_custom_msgs::object_loc apriltag_pos;

public:
	
    ardrone_autonomy::Navdata Read_navdata() 
    {
	mtx_.lock();
	ardrone_autonomy::Navdata _navdata = local_navdata;
	mtx_.unlock();
	return _navdata;
	}
    tum_ardrone::filter_state Read_state_estimation() 
    {
	mtx_.lock();
	tum_ardrone::filter_state _se = state_estimation;
	mtx_.unlock();
	return _se;
	}
    wasp_custom_msgs::object_loc Read_apriltag() 
    {
	mtx_.lock();
	wasp_custom_msgs::object_loc _apriltag_pos = apriltag_pos;
	mtx_.unlock();
	return _apriltag_pos;
	}
    void Update_navdata(const ardrone_autonomy::Navdata::ConstPtr& msg){
        mtx_.lock();
	local_navdata = *msg;
        mtx_.unlock();
    }
    void Update_stateEstimation(const tum_ardrone::filter_state::ConstPtr& msg){
        mtx_.lock();
	state_estimation = *msg;
        mtx_.unlock();
    }
    void Update_apriltag(const wasp_custom_msgs::object_loc::ConstPtr& msg){
        mtx_.lock();
        apriltag_pos = *msg;
        mtx_.unlock();
    }
} ardrone_navdata;


#endif
