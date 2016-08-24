//ArdroneNavdata.h
//Oskar
#include <boost/thread.hpp>

#ifndef _ARDRONE_NAVDATA
#define _ARDRONE_NAVDATA
class ArdroneNavdata {
private:
    boost::mutex mtx_;
    double rotX, rotY, rotZ;
    double altd;
    double vx,vy,vz;
    double ax,ay,az; //not updated or read yet
//Some more things maybe, tags etc.

public:
    double Read_rotX() 
    {
	mtx_.lock();
	double _rotX=rotX;
	mtx_.unlock();
	return _rotX;
    }

    double Read_rotY() 
    {
	mtx_.lock();
	double _rotY=rotY;
	mtx_.unlock();
	return _rotY;
    }

    double Read_rotZ() 
    {
	mtx_.lock();
	double _rotZ=rotZ;
	mtx_.unlock();
	return _rotZ;
    }

    double Read_altd() 
    {
	mtx_.lock();
	double _altd=altd;
	mtx_.unlock();
	return _altd;
    }
	
    double Read_vx() 
    {
	mtx_.lock();
	double _vx=vx;
	mtx_.unlock();
	return _vx;
    }


    double Read_vy() 
    {
	mtx_.lock();
	double _vy=vy;
	mtx_.unlock();
	return _vy;
    }

    double Read_vz() 
    {
	mtx_.lock();
	double _vz=vz;
	mtx_.unlock();
	return _vz;
    }

    void Update(){
        mtx_.lock();
        //Read new values on navdata
	//rotX = ...
	// ...
        mtx_.unlock();
    }
};

#endif
