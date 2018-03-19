# CarND-Path-Planning-Project
Self-Driving Car Engineer Nanodegree Program

## Overview

This README forms the project writeup from the Udacity Self Driving Car Nanodegree Path Planning project.
My solution consists of the following files:

File  |  Description
--|---|--
main.cpp  | The entry point and handler of messages from the simulator
Lane.cpp; .h  | Defines the Lane class that is used to represent each lane of the highway. Each lane is able to have a number of cars associated with it.
Car.cpp; .h  | Defines a Car class that represents each individual car forming the traffic encountered
Output.cpp; .h | Contains helper functions that output the traffic and car data to the terminal to allow debug and analysis
spline.h  | 3rd party cubic spline interpolation library used to plot a spline to the ego vehicle path

The project can be built using `cmake .. && make`
Note, the project was developed in Linux, and uses terminal window to output data relating to the ego vehicle and other traffic. Access to the console in windows is different and may cause errors.

### Model Documentation

#### Behavior Planner


The behaviour planner logic is split between the main.cpp and the lane.cpp files. In hindsight this should be refactored into a behaviour planner class (and Lane class) for clarity.

In the main.cpp file, upon receiving a message from the simulator, the message is decoded into the Ego vehicle data, and the sensor fusion data that represents the other cars on the highway. Frenet coordinates system is used to reprent the lateral and longitudinal position of the vehicles. The speed of the other vehicles on the road is calculated by taking the square root of the sum of the squares of the x and y  velocities.

Once the data has been decoded, a Car object is created to represent each of the other cars on the road, and they are then allocated to the appropriate lane object that represents each of the lanes.
The behaviour (i.e which lane to follow) is then determined by the analysis of the traffic on the highway. The Lane class is provided that represents a single lane of the highway, and includes the vehicles in that lane. In addition it provides multiple methods for reporting the relative position of the other vehicles in that lane, e.g their current S location and projected S position in Frenet at the point of the Ego vehicle's currently planned path being completed. Vehicles are categories as Threat Vehicles if their location means that they would be a hazard to the ego vehicle if it was in that lane )or to change into that lane). A cost method 'getLaneEfficencyCost' returns the efficency cost of that lane. Originally I combined the efficency and safety into a single cost function, but found that tuning the weights to be difficult. Hence I decided to use a 'hasThreatCars' property to represent if the lane was not safe. This is calculated by analysing the other vehicles position in relation to a defined threat zone that would place the vehilce near to the ego vehicle, but in addition, in particularly in order to prevent collisions at startup, the closing speed of the other vehicles in checked, and if the closing speed of a car behind is much greater than the ego vehicles speed, then it is also deemed unsafe.
The efficency cost is very simple, and is based on the nearest vehicle ahead of the ego car in that lanes distance and speed. If that vehicle is less than 50m ahead of the Ego vehicle a cost is created for the lane. This is intented to determine if the vehicle will be held up by the target vehilce. See code snippet below (note excessive casting to double due to some issues in development, I decided to explicitly cast all the weights)

```cpp
if (hasAheadCar) {
  if (nearest_ahead_car.delta_s < 50) {
    next_ahead_car_cost = (double)(((double)50.0-this->nearest_ahead_car_speed)/(double)50.0 + (double)2.0/nearest_ahead_car.delta_s);
  }
}
else
  next_ahead_car_cost =0;
```

After this, there is a slight benefit for being in the inside lane.

```cpp
if (laneNumber == 2 ){
  not_inside_lane_cost = 0;
}
else
{
  not_inside_lane_cost = 0.05;
}
```  

For efficency, it would be better if the vehicle was biased to stay in the centre lane where possible, however, this is illegal in my country (the UK) so I made the vehicle return to the inside lane whenever possible. This potentially causes inefficencies, as sometimes the best lane may be 2 lanes over. This can be modified by changing the weighting to apply to lane 1 (centre lane)

After all the sensor data has been processed, and each car in th etraffic added to th eappropriate lane, the evaluate() method can be called on each lane object to analyse the lane and calculate the costs.

One the most efficent lane is known, a very simple state machine using 2 states LaneChange and StayInLane is used to manage the lane changes. When the vehicle is in the StayInLane state, the efficency is checked to determine if the most efficent lane is not the current lane, and if so, attempt to change lanes. However, before changing lanes, the safety of the target lane is checked. In my mind, this is like an ADAS system overriding the more complex autonomy system of a real car, and providing a well established foundation of safety.
If the most efficent lane is not adjacent to the current lane, the planner not attempt to make the lane chaneg in one go, instead it will identify the next lane to go to, and make that transition. After completing that transition, it will reveryt o StayInLane, and re-evaluate the efficient of the lanes again, and then plan the next move. In most cases, the original target lane will still be the best lane so the vehicle will transition to that. I implemented it this way as I believe that this is the method that most drivers use to perform this operation. When in the act of changing lanes, the state machine will be LaneChange state. In this state the efficency is not checked again, but the safety is checked. Hence if a new hazard arises, such as another vehilce merging into that lane, the ego vehicle can abort and return to its original lane.

The ego vehilce speed is attempted to maintained as close to the speed limit as possible. If the ego vehicle is stuck in traffic behind another vehicle, it will use the sensor fusion data to get that vehicles actual speed and set its own target speed to match this.

Once the speed and target lane have been identified, a path is calculated using the method presented by Aaron Brown and David Silver in the walkthrough. In summary, the 3rd party spline library (from http://kluge.in-chemnitz.de/opensource/spline/) is used to plot a cubic spline to the targeted end point (the d of the centre of the target lane). 50 points are fitted to the spline. The ego car will attempt to reach each subsequent point every 0.02s, hence spacing of the points controls the vehicle speed.
Before creating the spline, the target points are converted to vehicle coordinates from Frenet.

The vehicle is able to navigate the track completely, completing the target distance of 4.32 miles without incident.
Ocasionally an accident can occur if a vehilce in an adjacent lane changes into the eg vehicle path if the closing speed is too great, meaning that there is not enough tine to slow. This could and should be avoided by predicting the behaviour of the vehilces in adjacent lanes, which I plan to update when I have time.

Below is a sample video of a section of journey.
<img src="https://github.com/Geordio/https://github.com/Geordio/CarND-Path-Planning-Project/master/data/pathplanning6.gif" alt="path"  width="341" height="250"/>




## Reference

The original udacity readme and setup instructions are below this point for completeness.

### Simulator.
You can download the Term3 Simulator which contains the Path Planning Project from the [releases tab (https://github.com/udacity/self-driving-car-sim/releases).

### Goals
In this project your goal is to safely navigate around a virtual highway with other traffic that is driving +-10 MPH of the 50 MPH speed limit. You will be provided the car's localization and sensor fusion data, there is also a sparse map list of waypoints around the highway. The car should try to go as close as possible to the 50 MPH speed limit, which means passing slower traffic when possible, note that other cars will try to change lanes too. The car should avoid hitting other cars at all cost as well as driving inside of the marked road lanes at all times, unless going from one lane to another. The car should be able to make one complete loop around the 6946m highway. Since the car is trying to go 50 MPH, it should take a little over 5 minutes to complete 1 loop. Also the car should not experience total acceleration over 10 m/s^2 and jerk that is greater than 10 m/s^3.

#### The map of the highway is in data/highway_map.txt
Each waypoint in the list contains  [x,y,s,dx,dy] values. x and y are the waypoint's map coordinate position, the s value is the distance along the road to get to that waypoint in meters, the dx and dy values define the unit normal vector pointing outward of the highway loop.

The highway's waypoints loop around so the frenet s value, distance along the road, goes from 0 to 6945.554.

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./path_planning`.

Here is the data provided from the Simulator to the C++ Program

#### Main car's localization Data (No Noise)

["x"] The car's x position in map coordinates

["y"] The car's y position in map coordinates

["s"] The car's s position in frenet coordinates

["d"] The car's d position in frenet coordinates

["yaw"] The car's yaw angle in the map

["speed"] The car's speed in MPH

#### Previous path data given to the Planner

//Note: Return the previous list but with processed points removed, can be a nice tool to show how far along
the path has processed since last time.

["previous_path_x"] The previous list of x points previously given to the simulator

["previous_path_y"] The previous list of y points previously given to the simulator

#### Previous path's end s and d values

["end_path_s"] The previous list's last point's frenet s value

["end_path_d"] The previous list's last point's frenet d value

#### Sensor Fusion Data, a list of all other car's attributes on the same side of the road. (No Noise)

["sensor_fusion"] A 2d vector of cars and then that car's [car's unique ID, car's x position in map coordinates, car's y position in map coordinates, car's x velocity in m/s, car's y velocity in m/s, car's s position in frenet coordinates, car's d position in frenet coordinates.

## Details

1. The car uses a perfect controller and will visit every (x,y) point it recieves in the list every .02 seconds. The units for the (x,y) points are in meters and the spacing of the points determines the speed of the car. The vector going from a point to the next point in the list dictates the angle of the car. Acceleration both in the tangential and normal directions is measured along with the jerk, the rate of change of total Acceleration. The (x,y) point paths that the planner recieves should not have a total acceleration that goes over 10 m/s^2, also the jerk should not go over 50 m/s^3. (NOTE: As this is BETA, these requirements might change. Also currently jerk is over a .02 second interval, it would probably be better to average total acceleration over 1 second and measure jerk from that.

2. There will be some latency between the simulator running and the path planner returning a path, with optimized code usually its not very long maybe just 1-3 time steps. During this delay the simulator will continue using points that it was last given, because of this its a good idea to store the last points you have used so you can have a smooth transition. previous_path_x, and previous_path_y can be helpful for this transition since they show the last points given to the simulator controller with the processed points already removed. You would either return a path that extends this previous path or make sure to create a new path that has a smooth transition with this last path.

## Tips

A really helpful resource for doing this project and creating smooth trajectories was using http://kluge.in-chemnitz.de/opensource/spline/, the spline function is in a single hearder file is really easy to use.

---

## Dependencies

* cmake >= 3.5
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `install-mac.sh` or `install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets
    cd uWebSockets
    git checkout e94b6e1
    ```

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!


## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./

## How to write a README
A well written README file can enhance your project and portfolio.  Develop your abilities to create professional README files by completing [this free course](https://www.udacity.com/course/writing-readmes--ud777).
