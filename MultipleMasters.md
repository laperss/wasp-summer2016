Simple example of using `multimaster_fkie` package. It makes it
possible to have multiple masters communicating.

Webpage: http://wiki.ros.org/multimaster_fkie/Tutorials/
### 1. Download and install the package 

```
cd ~/catkin_ws/src
git clone https://github.com/fkie/multimaster_fkie.git
cd ..
catkin_make
```

### 2. Simple example with one host: 

You need two terminal windows open.

In first terminal window we will start a ROS master and an example
publisher, which publish `Hello World` to topic with name
`/test/topic`. Note that this master is communicating on port 11311.

```
export ROS_MASTER_URI=http://localhost:11311
roscore >/dev/null2>&1 &
rostopic pub /test/topic std_msgs/String 'Hello World' -r 1 &
```

In the second terminal we will try to subscribe to this topic but with
a master communicating on a different port.

```
export ROS_MASTER_URI=http://localhost:11312
roscore --port 11312 >/dev/null 2>&1 &
rostopic echo /test/topic &
```

At this moment nothing will be published on the master at
`localhost:11312` so nothing should be printed in the second terminal.

Now we will start a `master_discovery` node and a `master_sync` node
in both terminals which will make it possible for the two masters to
find each other and communicate with each other,

In 1st terminal:

```
rosrun master_discovery_fkie master_discovery >/dev/null 2>&1 &
rosrun master_sync_fkie master_sync >/dev/null 2>&1 &
```

In 2nd terminal:

```
rosrun master_discovery_fkie master_discovery >/dev/null 2>&1 &
rosrun master_sync_fkie master_sync >/dev/null 2>&1 &
```

Now both ROS-masters are synchronized and the second terminal should
start printing the `Hello World` message.

### 3. Do it with turtlebot and ar drone


On the turtlebot pc (install the package first) we need to run the
following in three different terminal windows:

```
roslaunch turtlebot_bringup minimal.launch >/dev/null 2>&1 &
rosrun master_discovery_fkie master_discovery >/dev/null 2>&1 &
rosrun master_sync_fkie master_sync  >/dev/null 2>&1 &
```

Then run the same on the other pc with the master connected to the AR
Drone. Check all the `rostopics` to see if it worked:

```
rostopic list -v
```

