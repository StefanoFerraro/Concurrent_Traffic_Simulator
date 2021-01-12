#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <vector>
#include <future>
#include <mutex>
#include <memory>
#include "TrafficObject.h"
#include "TrafficLight.h"


class Street;
class Vehicle;
class TrafficLight;
//enum trafficLightPhase;

class WaitingVehicles
{
    public:
        int getSize();
        void pushBack(std::shared_ptr<Vehicle> vehicle, std::promise<void> &&promise);
        void permitEntryToFirstInQueue();

    private:
        std::vector<std::shared_ptr<Vehicle>> _vehicles;
        std::vector<std::promise<void>> _promises; //promises are needed in order to comunicate with the main function that the status of the vehicle
        std::mutex _mutex;
};

class Intersection: public TrafficObject, public std::enable_shared_from_this<Intersection>
{
    public:
        Intersection();
        
        void addVehicleToQueue(std::shared_ptr<Vehicle> vehicle);
        void addStreet(std::shared_ptr<Street> street);
        std::vector<std::shared_ptr<Street>> queryStreets(std::shared_ptr<Street> incoming);
        void simulate();
        void vehicleHasLeft(std::shared_ptr<Vehicle> vehicle);
        trafficLightPhase trafficLightColor();

    private:

        void processVehicleQueue();     // method for processing the queue (letting new vehicle in)
        std::vector<std::shared_ptr<Street>> _streets;  //pool of streets that as the current intersection as an edge
        WaitingVehicles _waitingVehicles;  // vehicles waiting to enter the intersection
        bool _isBlocked;    //marker for blocking the intersection to other vehicles
        std::shared_ptr<TrafficLight> _trafficLight;
};

#endif
