#include <iostream>
#include <random>
#include <cmath>
#include "Street.h"  
#include "Intersection.h"
#include "Vehicle.h"

Vehicle::Vehicle()
{
    _type = ObjectType::objectVehicle;
    _speed = 400;    // in m/s
    _posStreet = 100.0;
    _currStreet = nullptr; 
}

void Vehicle::setCurrentDestination(std::shared_ptr<Intersection> destination)
{
    _currDestination = destination;
    _posStreet = 0.0; // Every time a new destination is given we want to reset the position to 0.0
}

void Vehicle::simulate()
{   
    // We add a thread to the vector, giving the function "drive" to execute
    threads.emplace_back(std::thread(&Vehicle::drive, this)); //specific notation when wanting to pass a method of a class, applied to a a spedific instance of the class
}

// function for simulating the vehicle movement along the street
void Vehicle::drive()
{
    // We need to consider three scenario:
    // 1. normal speed, vehicle if far from the destination intersection
    // 2. intersection approach, spped is slowed down to a factor of 10
    // 3. car exit intersection and speed is reset to normal

    std::unique_lock<std::mutex> lck(_mtx); // unique lock to the mutex created in the TrafficObject class, main purpose is the cout operation
    std::cout << "Vehicle #" << _id << "::drive: thread id = " << std::this_thread::get_id() << std::endl;
    lck.unlock();

    // initialization
    bool hasEnterIntersection = false;
    double cycleDuration = 1; // duration of a single simulation cycle in ms
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;
    lastUpdate = std::chrono::system_clock::now();

    while(true) // once started cycle runs continuosly
    {   
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); //sleep at every cycle reduce the CPU usage (increase sleep time) and help expose data races

        long deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        if(deltaTime >= cycleDuration)
        {
            _posStreet += _speed * deltaTime / 1000; // the operator / returns an int

            double percCompletition = _posStreet / _currStreet->getLength();

            std::shared_ptr<Intersection> in = _currDestination->getID() == _currStreet->getInIntersection()->getID() ? _currStreet->getOutIntersection() : _currStreet->getInIntersection(); 
            std::shared_ptr<Intersection> out = _currDestination;

            double x1, y1, x2, y2, dx, dy, l, xv, yv;

            in->getPosition(x1, y1);
            out->getPosition(x2, y2);

            dx = x2 - x1;
            dy = y2 - y1;

            // l = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));

            xv = x1 + percCompletition * dx;
            yv = y1 + percCompletition * dy;

            this->setPosition(xv, yv);

            // check whenever the Vehicle is approaching the intersection
            if(percCompletition >= 0.9 && !hasEnterIntersection)  
            {
                // create a future for starting a task and adding the vehicle to the queue of the intersection of destination
                // AddVehicleToQueue will allow acce to the intersection only when the vehicle is first in queue 
                auto ftrEntryGranted = std::async(&Intersection::addVehicleToQueue, _currDestination, get_shared_this());
                
                // wait for the task to return to the future made 
                ftrEntryGranted.get();
                
                // decrease speed
                _speed /= 10.0;
                hasEnterIntersection = true;
            }

            if(percCompletition >= 1.0 && hasEnterIntersection)
            {
                // choose next street and destination for the vehicle
                std::vector<std::shared_ptr<Street>> streetOptions = _currDestination->queryStreets(_currStreet); // queryStreets return a list of the available streets to take, exluding the street of it came
                std::shared_ptr<Street> nextStreet;

                // Handling exception of a no-way-out street
                if (streetOptions.size() > 0)
                {
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std:: uniform_int_distribution<> dis(0, streetOptions.size() - 1);

                    nextStreet = streetOptions.at(dis(gen));
                } 

                else
                {
                    nextStreet = _currStreet;
                }

                // pick next destination as the intersection in the nextStreet selected that is opposite to the current one
                std::shared_ptr<Intersection> nextDestination = _currDestination->getID() == nextStreet->getInIntersection()->getID() ? nextStreet->getOutIntersection() : nextStreet->getInIntersection();

                _currDestination->vehicleHasLeft(get_shared_this());

                this->setCurrentDestination(nextDestination);
                this->setCurrentStreet(nextStreet);

                // reset speed
                _speed *= 10.0;
                hasEnterIntersection = false;
            }

            // update clock for next cycle
            lastUpdate = std::chrono::system_clock::now();
        }
    }
}



    

