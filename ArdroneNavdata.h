//ArdroneNavdata.h
//Oskar
/*
Datamonitor to store the navdata from the ardrone. It stores the
navdata as is, i.e. as an "ardrone_autonomy::Navdata"
 */

#include <boost/thread.hpp>
#include <ardrone_autonomy/Navdata.h>
#include <dynamic_reconfigure/ConfigDescription.h>
#include <tum_ardrone/filter_state.h>


#ifndef _ARDRONE_NAVDATA
#define _ARDRONE_NAVDATA


class ArdroneNavdata {
private:
    boost::mutex mtx_;
    ardrone_autonomy::Navdata local_navdata;
    tum_ardrone::filter_state state_estimation;

public:
    float x, y, z, roll, pitch, yaw, dyaw, battery;
    int state;
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
    void Update_navdata(const ardrone_autonomy::Navdata::ConstPtr& msg){
        mtx_.lock();
	local_navdata = *msg;
        mtx_.unlock();
    }
    void Update_stateEstimation(const tum_ardrone::filter_state::ConstPtr& msg){
        mtx_.lock();
	state_estimation = *msg;
        mtx_.unlock();
	x = state_estimation.x;
	y = state_estimation.y;
	z = state_estimation.z;
	roll = state_estimation.roll;
	pitch = state_estimation.pitch;
	yaw = state_estimation.yaw;
	dyaw = state_estimation.dyaw;
	state = state_estimation.droneState;
	battery = state_estimation.batteryPercent;
    }
} ardrone_navdata;


#endif
