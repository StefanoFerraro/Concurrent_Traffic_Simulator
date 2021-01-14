#ifndef VEHICLE_H
#define VEHICLE_H

#include "TrafficObject.h"

// Forward declarations to be used in current class
class Intersection;
class Street;

class Vehicle : public TrafficObject, public std::enable_shared_from_this<Vehicle>
{
    public:
        Vehicle(); //desctructor already handled in the TrafficObject class 

        void setCurrentStreet(std::shared_ptr<Street> street) { _currStreet = street;}
        std::shared_ptr<Street> getCurrentStreet() {return _currStreet;}
        void setCurrentDestination(std::shared_ptr<Intersection> destination);

        void simulate();
        
        std::shared_ptr<Vehicle> get_shared_this() { return shared_from_this(); }

    private:

        void drive(); //function to pass to the thread for simulating the vehicle movement along the street
        std::shared_ptr<Intersection> _currDestination;
        std::shared_ptr<Street> _currStreet;
        double _posStreet;
        double _speed; // in m/s
};

#endif