#include <iostream>
#include "Intersection.h"
#include "Street.h"

Street::Street()
{
    _type = ObjectType::objectStreet;
    _length = 1000;
}

void Street::setInIntersection(std::shared_ptr<Intersection> in)
{
    _interIn = in;
}

void Street::setOutIntersection(std::shared_ptr<Intersection> out)
{
    _interOut = out;
}

