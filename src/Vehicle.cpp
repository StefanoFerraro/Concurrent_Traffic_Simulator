#include <iostream>
#include <random>
#include "Street.h"
#include "Intersection.h"
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
}

//TO DO