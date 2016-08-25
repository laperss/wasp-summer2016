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
#include <vector>
#ifndef _ARDRONE_NAVDATA
#define _ARDRONE_NAVDATA

class ArdroneNavdata {

private:

    boost::mutex mtx_;
    ardrone_autonomy::Navdata local_navdata;
    tum_ardrone::filter_state state_estimation;
    //wasp_custom_msgs::object_loc apriltag_pos;
    std::vector<wasp_custom_msgs::object_loc> object_vector;


    
 public:

    bool does_object_exists(wasp_custom_msgs::object_loc obj)
    {
      bool exists=false;
  
      mtx_.lock();
      std::vector<wasp_custom_msgs::object_loc> obj_vec_loc = object_vector;
      mtx_.unlock();
  
      std::vector<wasp_custom_msgs::object_loc>::iterator it;
      for(it = obj_vec_loc.begin(); it!=obj_vec_loc.end(); it++)
	{
	  if(it->ID == obj.ID)
	    {
	      exists=true;
	    }
	}

      return exists;
    }
	
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

    std::vector<wasp_custom_msgs::object_loc> get_objects() 
    {
	mtx_.lock();
	std::vector<wasp_custom_msgs::object_loc> objvec = object_vector;
	mtx_.unlock();
	return objvec;
    }

  
    void Update_stateEstimation(const tum_ardrone::filter_state::ConstPtr& msg)
    {
        mtx_.lock();
	state_estimation = *msg;
        mtx_.unlock();
    }

   
    void Update_objectlist(const wasp_custom_msgs::object_loc::ConstPtr& msg)
    {
        wasp_custom_msgs::object_loc obj = *msg;
	if(!does_object_exists(obj))
	  {
	    mtx_.lock();
	    object_vector.push_back(obj);
	    mtx_.unlock();
	  }	
    }

} ardrone_navdata;


#endif
