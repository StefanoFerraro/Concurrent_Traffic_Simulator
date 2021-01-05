#include <iostream>
#include "Vehicle.h"
#include "Intersection.h"
#include "Street.h"

Street::Street()
{
    _type = ObjectType::objectStreet;
    _length = 1000.0;
}

void setInIntersection(std::shared_ptr<Intersection> in)
{
    _interIn = in;
    in->addStreet(get_shared_this());   // add current instance of Street to the Intersection list of streets that are part of an intersection
}

void setOutIntersection(std::shared_ptr<Intersection> out);
{
    _interOut = out;
    out->addStreet(get_shared_this());  // add current instance of Street to the Intersection list of streets that are part of an intersection
}