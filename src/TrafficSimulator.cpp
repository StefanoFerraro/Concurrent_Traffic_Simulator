#include <iostream>
#include <thread>
#include <vector>

#include "Intersection.h"

void createTrafficObject(std::vector<std::shared_ptr<Street>> &streets, std::vector<std::shared_ptr<Intersection>> &intersections, std::vector<std::shared_ptr<Vehicle>> &vehicles, int nVehicles)
{
    std::string filename = "data/nyc.jpg";

    int nIntersections = 6;

    for (size_t i = 0; i < nIntersections; i++)
    {
        intersections.push_back(std::make_shared<Intersection>());
    }

    // TO DO


}

int main()
{
    std::vector<std::shared_ptr<Street>>  streets;
    std::vector<std::shared_ptr<Intersection>>  intersections;
    std::vector<std::shared_ptr<Vehicle>>  vehicles;

    int nVehicles = 5;

    createTrafficObject(streets, intersections, vehicles, nVehicles);


}