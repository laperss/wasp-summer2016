//ArdroneNavdata.h
//Oskar
/*
Datamonitor to store the navdata from the ardrone. It stores the
navdata as is, i.e. as an "ardrone_autonomy::Navdata"
 */

#include <boost/thread.hpp>
#include <ardrone_autonomy/Navdata.h>

#ifndef _ARDRONE_NAVDATA
#define _ARDRONE_NAVDATA


class ArdroneNavdata {
private:
    boost::mutex mtx_;
    ardrone_autonomy::Navdata local_navdata;

public:
    ardrone_autonomy::Navdata Read_navdata() 
    {
	mtx_.lock();
	ardrone_autonomy::Navdata _navdata = local_navdata;
	mtx_.unlock();
	return _navdata;
	}
    void Update(const ardrone_autonomy::Navdata::ConstPtr& msg){
        mtx_.lock();
	local_navdata = *msg;
        mtx_.unlock();
    }
} ardrone_navdata;


#endif
