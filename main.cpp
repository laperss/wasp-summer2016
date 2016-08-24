//main.cpp
//Oskar

#include "boost/thread.hpp"
#include "SubScriber.cpp"
#include "RunDrone.cpp" //here we need a Control_Thread here

int main() 
{
    boost::thread thread_subscriber(Subscribe_Thread); // start concurrent execution of bankAgent
    boost::thread thread_controller(Control_Thread); // start concurrent execution of Joe
    thread_subscriber.join();
    thread_controller.join();
    return 0;
}
