#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <vector>
#include <future>
#include <mutex>
#include <memory>
#include "TrafficObject.h"


// forward declaration to avoid include cycle
class Street;
class Vehicle;

// class for queue and dequeue waiting vehicles at an intersection
class WaitingVehicles
{
    public:
        int getSize();

        void pushBack(std::shared<Vehicle> vehicle, std::promise<void> &&promise)
        void permitEntryToFirstInQueue();

    private:
        std::vector<std::shared_ptr<Vehicle>> _vehicles;
        std::vector<std::promise<void>> _promises;
        std::mutex _mutex;
};

class Intersection : public TrafficObject
{
    public:
        Intersection();

        void setIsBlocked(bool isBlocked);

        void addVehicleToQueue(std::shared_ptr<Vehicle> vehicle);
        void addStreet(std::shared_ptr<Street> street);
        std::vector<std::shared_ptr<Street>> queryStreets(std::shared_ptr<Street> incoming); // return pointer to current list of all outgoing streets
        void simulate();
        void vehicleHasLeft(std::shared_ptr<Vehicle> vehicle);
        bool trafficLightIsGreen();

    private:
        void processVehicleQueue();
        std::vector<std::shared_ptr<Street>> _streets;
        WaitingVehicles _waitingVehicles;
        bool _isBlocked;
}

#endif
