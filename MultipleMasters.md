To have multiple masters one should follow this :)

This package will make different masters share topics over the
ros-network so that every master can publish/subscribe to all topics :)

For our purposes this will make it possible to have one roscore-master
running on Linneas compuert (which has all the ardrone topics but no
turtlebot topics) and one roscore-master running on the turtlebot
computer (we have to have one there in order to control the cameras
and that stuff). We then start the magic from this package and it will
be possible to see all the turtlebot topics on Linneas computer.

first you have to install a package fkie_multimasters:

1. Download and install the package on all computers where you will
have a ros_master running:

```
$ git clone https://github.com/fkie/multimaster_fkie.git
$ cd ..
$ catkin_make
```

2. Test it with a simple example if you wish. Follow the tutorial on
http://wiki.ros.org/multimaster_fkie/Tutorials/

3. On the turtlebot pc (the package is installed) we need to run the following in three different terminal windows:

```
terminal1$ roslaunch turtlebot_bringup minimal.launch 
terminal2$ rosrun master_discovery_fkie master_discovery
treminal3$ rosrun master_sync_fkie master_sync
```

Alternatively one could do it like this and use only a single
terminal. This will start the processes and run them silently in the
background:

```
$ roslaunch turtlebot_bringup minimal.launch >/dev/null 2>&1 &
$ rosrun master_discovery_fkie master_discovery >/dev/null 2>&1 &
$ rosrun master_sync_fkie master_sync  >/dev/null 2>&1 &
```

If you wish to kill all the processes created by the user (turtlebot)
and logout:
```
$pkill -u turtlebot
```
