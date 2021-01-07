#include <iostream>
#include <random>
#include "Street.h"ffe   
#include "Intersection.h"e
#include "Vehicle.h"

Vehicle::Vehicle()
{
    _currStreet = nullptr;
    _posStreet = 0.0;
    _type = ObjectType::objectVehicle;
    _speed = 400;
}

void Vehicle::setCurrentDestination(std::shared_ptr<Intersection> destination)
{
    _currDestination = destination;
    _posStreet = 0.0;
}

void Vehicle::simulate()
{
    threads.emplace_back(std::thread(&Vehicle::drive, this));   // we execute the drive function of the current instance of vehicle, and the thread is added to the vector of threads 
}

void Vehicle::drive()
{
    std::unique_lock<std::mutex> lck(_mtx);
    std::cout << "Vehicle #" << _id << "::drive: thread id = " << std::this_thread::get_id() << std::endl;
    lck.unlock();

    bool hasEnteredIntersection = false;
    double cycleDuration = 1;
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;

    lastUpdate = std::chrono::system_clock::now();
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        if (timeSinceLastUpdate >= cycleDuration)
        {
            // Update position
            _posStreet += _speed*timeSinceLastUpdate / 1000; 

            double completition = _posStreet / _currStreet->getLength();

            // compute current pixel position on street based on driving direction
            std::shared_ptr<Intersection> i1, i2;
            i2 = _currDestination;
            i1 = i2->getID() == _currStreet->getInIntersection()->getID() ? _currStreet->getOutIntersection() : _currStreet->getInIntersection();

            double x1, y1, x2, y2, xv, yv, dx, dy, l;
            i1->getPosition(x1, y1);
            i2->getPosition(x2, y2);
            dx = x2 - x1;
            dy = y2 -y1;
            l = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));

            xv = x1 + completition*dx;
            yv = y1 + completition*dy;
            this->setPosition(xv, xy);

            // check wether halting position in front of destination has been reached
            if(completition >= 0.9 && !hasEnteredIntersection)
            {
                auto ftrEntryGranted = std::async(&Intersection::addVehicleToQueue, _currDestination, get_shared_this());

                ftrEntryGranted.get(); //ftrEntryGranted gets signal that the access has been granted 

                // slow down and set intersection flag 
                _speed /= 10.0;
                hasEnteredIntersection = true;
            }

            if (completition >= 1.0 && hasEnteredIntersection)
            {
                std::vector<std::shared_ptr<Street>> streeOptions = _currDestination->queryStreets(_currStreet);
                std::shared_ptr<Street> nextStreet;
                if(streeOptions.size() > 0) //pick next street rndomlly
                {
                    std::random_device rd;
                    std::mt19937 eng(rd());
                    std::uniform_int_distribution<> distr(0, streetOptions.size() - 1);
                    nextStreet = streeOptions.at(distr(eng));
                }
                else
                {
                    nextStreet = _currStreet; // in case the street is a dead-end
                }
                
                // for next destination pick the intersection at which the vehicle is not
                std::shared_ptr<Intersection> nextIntersection = nextStreet->getIntersection()->getID() == _currDestination->getID() ? nextStreet->getOutIntersection() : nextStreet->getInIntersection(); 

                //send signal to intersection that vehicle has left the intersection
                _currDestination->vehicleHasLeft(get_shared_this());

                // assign new street and destination
                this->setCurrentdestination(nextIntersection);
                this->setCurrentStreet(nextStreet);

                // reset parameter at exit of intersection
                _speed *= 10.0;
                hasEnteredIntersection = false;
            }
            // reset stop watch
            lastUpdate = std::chrono::system_clock::now();
        }
    }
}