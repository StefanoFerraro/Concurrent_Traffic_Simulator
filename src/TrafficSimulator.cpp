#include <iostream>
#include <thread>
#include <vector>

#include "Intersection.h"
#include "Graphics.h"
#include "Vehicle.h"
#include "Street.h"

void createTrafficObject(std::vector<std::shared_ptr<Street>> &streets, std::vector<std::shared_ptr<Intersection>> &intersections, std::vector<std::shared_ptr<Vehicle>> &vehicles, std::string &filename, int nVehicles)
{
    filename = "../data/nyc.jpg";

    int nIntersection = 10;

    for(size_t ni = 0; ni < nIntersection; ni++)
    {   
        intersections.push_back(std::make_shared<Intersection>());
    }

    // position intersections in pixel coordinates
    intersections.at(0)->setPosition(1430, 625);
    intersections.at(1)->setPosition(2575, 1260);
    intersections.at(2)->setPosition(2200, 1950);
    intersections.at(3)->setPosition(1000, 1350);
    intersections.at(4)->setPosition(400, 1000);
    intersections.at(5)->setPosition(750, 250);
    intersections.at(6)->setPosition(2775, 885);
    intersections.at(7)->setPosition(1630, 250);
    intersections.at(8)->setPosition(850, 1650);
    intersections.at(9)->setPosition(300, 1370);

    int nStreet = 13;

    for(size_t ns = 0; ns < nStreet; ns++)
    {   
        streets.push_back(std::make_shared<Street>());
    }

    // streets.at(0)->setInIntersection(intersections.at(0));
    // streets.at(0)->setOutIntersection(intersections.at(1));
    streets.at(0)->setStreet(intersections.at(0), intersections.at(1));
    
    // streets.at(1)->setInIntersection(intersections.at(1));
    // streets.at(1)->setOutIntersection(intersections.at(2));
    streets.at(1)->setStreet(intersections.at(1), intersections.at(2));

    // streets.at(2)->setInIntersection(intersections.at(2));
    // streets.at(2)->setOutIntersection(intersections.at(3));
    streets.at(2)->setStreet(intersections.at(2), intersections.at(3));

    // streets.at(3)->setInIntersection(intersections.at(3));
    // streets.at(3)->setOutIntersection(intersections.at(4));
    streets.at(3)->setStreet(intersections.at(3), intersections.at(4));

    // streets.at(4)->setInIntersection(intersections.at(4));
    // streets.at(4)->setOutIntersection(intersections.at(5));
    streets.at(4)->setStreet(intersections.at(4), intersections.at(5));

    // streets.at(5)->setInIntersection(intersections.at(5));
    // streets.at(5)->setOutIntersection(intersections.at(0));
    streets.at(5)->setStreet(intersections.at(5), intersections.at(0));

    // streets.at(6)->setInIntersection(intersections.at(0));
    // streets.at(6)->setOutIntersection(intersections.at(3));
    streets.at(6)->setStreet(intersections.at(0), intersections.at(3));

    // streets.at(7)->setInIntersection(intersections.at(1));
    // streets.at(7)->setOutIntersection(intersections.at(6));
    streets.at(7)->setStreet(intersections.at(1), intersections.at(6));

    // streets.at(8)->setInIntersection(intersections.at(6));
    // streets.at(8)->setOutIntersection(intersections.at(7));
    streets.at(8)->setStreet(intersections.at(6), intersections.at(7));

    // streets.at(9)->setInIntersection(intersections.at(7));
    // streets.at(9)->setOutIntersection(intersections.at(0));
    streets.at(9)->setStreet(intersections.at(7), intersections.at(0));

    // streets.at(10)->setInIntersection(intersections.at(3));
    // streets.at(10)->setOutIntersection(intersections.at(8));
    streets.at(10)->setStreet(intersections.at(3), intersections.at(8));

    // streets.at(11)->setInIntersection(intersections.at(8));
    // streets.at(11)->setOutIntersection(intersections.at(9));
    streets.at(11)->setStreet(intersections.at(8), intersections.at(9));

    // streets.at(12)->setInIntersection(intersections.at(9));
    // streets.at(12)->setOutIntersection(intersections.at(4));
    streets.at(12)->setStreet(intersections.at(9), intersections.at(4));


    for(size_t nv = 0; nv < nVehicles; nv++)
    {
        vehicles.push_back(std::make_shared<Vehicle>());

        // Place each vehicle at a specific street with a given destination
        vehicles.at(nv)->setCurrentStreet(streets.at(nv%nStreet));  //place one vehicle per street
        //if (nv%6 == 0) {vehicles.at(nv)->setCurrentDestination(intersections.at(0));} //handling specific case for the sixth street 
        //else {vehicles.at(nv)->setCurrentDestination(intersections.at(nv%(5)));} // limit the number of vehicles, if nv == 6, no intersection is identified
        vehicles.at(nv)->setCurrentDestination(vehicles.at(nv)->getCurrentStreet()->getOutIntersection());
    }
}

int main()
{
    /* PART 1 : Set up traffic objects */
    std::vector<std::shared_ptr<Street>>  streets;
    std::vector<std::shared_ptr<Intersection>>  intersections;
    std::vector<std::shared_ptr<Vehicle>>  vehicles;

    std::string backgroundImg;

    int nVehicles = 10;

    createTrafficObject(streets, intersections, vehicles, backgroundImg,  nVehicles);

    /* PART 2 : Simulate traffic objects */

    // We divide the simulation in two side, one for intersection (handle the queue at each intersection)
    // and a second for the vehicles movement around the map

    // start simulation of intersections (each intersection a thread so the main function goes on with the execution)
    std::for_each(intersections.begin(), intersections.end(), [](std::shared_ptr<Intersection> &i){i->simulate();});

    // start simulation of vehicles (each vehicle a thread so the main function goes on with the execution)
    std::for_each(vehicles.begin(), vehicles.end(), [](std::shared_ptr<Vehicle> &v){v->simulate();});

    /* PART 3 : Start the Graphic Interface Simulation*/

    std::vector<std::shared_ptr<TrafficObject>> trafficObjects; // the graphical unit require a vector containg Intersections and Streets. All the element of the vector are of class TrafficObjects
    std::for_each(intersections.begin(), intersections.end(), [&trafficObjects](std::shared_ptr<Intersection> &i){
        std::shared_ptr<TrafficObject> TObj = std::dynamic_pointer_cast<TrafficObject>(i); // we cast the object before pushing it into the vector
        // C++ checks if the casting we are doing is actually valid, and it can be performed just between inherited classes and the parent.
        trafficObjects.push_back(TObj);
        });

    std::for_each(vehicles.begin(), vehicles.end(), [&trafficObjects](std::shared_ptr<Vehicle> &v){
        std::shared_ptr<TrafficObject> TObj = std::dynamic_pointer_cast<TrafficObject>(v); // we cast the object before pushing it into the vector
        // C++ checks if the casting we are doing is actually valid, and it can be performed just between inherited classes and the parent.
        trafficObjects.push_back(TObj);
        });

    Graphics *graphics = new Graphics();

    graphics->setBgFilename(backgroundImg);
    graphics->setTrafficObjects(trafficObjects);
    graphics->simulate(); // while loop, receives data from the vehicle and intersection threads
}