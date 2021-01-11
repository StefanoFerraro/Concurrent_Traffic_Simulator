#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <random>

#include "Street.h"
#include "Intersection.h"
#include "Vehicle.h"

int WaitingVehicles::getSize() 
{
    // before accessing this critical information we want to block the access of the resource to others, in order to avoid data races
    std::lock_guard<std::mutex> lck(_mutex); //the lock guard manages the mutex when the function expire, ew do not have to worry about lock/unlocking
    return _vehicles.size();
}

void WaitingVehicles::pushBack(std::shared_ptr<Vehicle> vehicle, std::promise<void> &&promise)
{
    // before accessing this critical information we want to block the access of the resource to others, in order to avoid data races
    std::lock_guard<std::mutex> lck(_mutex); //the lock guard manages the mutex when the function expire, ew do not have to worry about lock/unlocking
    _vehicles.push_back(vehicle);
    _promises.push_back(std::move(promise));
}

void WaitingVehicles::permitEntryToFirstInQueue()
{
    // before accessing this critical information we want to block the access of the resource to others, in order to avoid data races
    std::lock_guard<std::mutex> lck(_mutex); //the lock guard manages the mutex when the function expire, ew do not have to worry about lock/unlocking
    
    _promises.begin()->set_value(); // return to the future, vehicle has entered the intersection

    _vehicles.erase(_vehicles.begin());
    _promises.erase(_promises.begin());
}

Intersection::Intersection()
{
    _type = ObjectType::objectIntersection;
    _isBlocked = false;

}

void Intersection::addVehicleToQueue(std::shared_ptr<Vehicle> vehicle)
{
    std::unique_lock<std::mutex> lck(_mtx);
    std::cout << "Intersection #" << _id << "::addVehicleToQueue: thread id = " << std::this_thread::get_id() << std::endl;
    lck.unlock(); //unlock the cout static mutex in case other has to stamp

    std::promise<void> prms;
    std::future<void> ftr = prms.get_future();
    _waitingVehicles.pushBack(vehicle, std::move(prms));

    ftr.wait(); // blocks the thread and wait for the vehicle to exit the intersection

    lck.lock();
    std::cout << "Intersection #" << _id << ": Vehicle #" << vehicle->getID() << " is granted entry." << std::endl;

        // FP.6b : use the methods TrafficLight::getCurrentPhase and TrafficLight::waitForGreen to block the execution until the traffic light turns green.

    lck.unlock();

}

void Intersection::addStreet(std::shared_ptr<Street> street)
{
    _streets.push_back(street);
}

void Intersection::vehicleHasLeft(std::shared_ptr<Vehicle> vehicle)
{
    _isBlocked = false;
}

std::vector<std::shared_ptr<Street>> Intersection::queryStreets(std::shared_ptr<Street> incoming)
{
    std::vector<std::shared_ptr<Street>> out;
    
    for (auto i : _streets)
    {
        if (i->getID() != incoming->getID()) //exclude incoming street to the streets to output as possible next destinations
        {
            out.push_back(i);
        }
    } 

    return out;
}

void Intersection::simulate()
{
    // in order to simulate the intersections, we are going to create a thread for the current element of the class
    threads.emplace_back(std::thread(&Intersection::processVehicleQueue, this)); //when passing a methos of a class we specify also the instance where to apply the method 
}

void Intersection::processVehicleQueue()
{
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if(_waitingVehicles.getSize() > 0 && !_isBlocked)
        {
            _isBlocked = true;
            _waitingVehicles.permitEntryToFirstInQueue();
        }
    }
}

bool Intersection::trafficLightIsGreen()
{
    return true;
}

