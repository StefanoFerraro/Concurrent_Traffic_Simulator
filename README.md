# Concurrent Traffic Simulation

<img src="data/traffic_simulation.gif"/>


This project is part of an advanced course taken at: [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The focus of the project is multithreading and thread-safe communication protocols. 

In the simulator each intersection, traffic light and vehicle are distinct threads that runs concurrently.  
Each vehicle is responsible for adjusting the speed with respect to the position in the street. Reached the intersection a separate task is responsible for adding the vehicle to the intersection queue. At the intersection it waits for authorization from the relative intersection thread before entering.   
Each intersection based on the phase of the traffic light add or process the FIFO queue of the vehicles waiting to enter.   
Each traffic light is responsible for cycling through a 4 to 6 seconds red or green phase and communicating the status to the relative intersection thread.

Below an overview of the data flow of the program, green dotted lines are thread creations and in the red you have the data exchange between threads.

![Pseudo-code Schematic](data/schematic.png)

Main C++ features used in the project are:
- **Object-Oriented-Programming**: all the objects cited are inherited of a class `trafficObject`, each thread is an instance of a specific class
- **Shared pointers**: for traking the ownership, and passing it to multiple entities automatically managing the deallocation process from the heap
- **Threads**: parallel execution of differect functions; data exchange between threads (promises, futures)
- **Mutex**: to avoid data races between threads and having a stable long-term communication channel
- **Condition Variables**: to establish a message queue between intersections and traffic light, in order to freeze the execution of a thread till a new traffic light phase is not available 

## Dependencies for Running Locally
* cmake >= 2.8
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* OpenCV >= 4.1
  * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./traffic_simulation`.
