#include <iostream>
#include "Intersection.h"
#include "Street.h"

Street::Street()
{
    _type = ObjectType::objectStreet;
    _length = 1000;
}

void Street::setInIntersection(std::shared_ptr<Intersection> &in) // since we need to add the connected streets to the intersection, we need to pass the pointer as a referece
{
    _interIn = in;
    in->addStreet(shared_from_this());  // share the current street to the intersection

}

void Street::setOutIntersection(std::shared_ptr<Intersection> &out) // since we need to add the connected streets to the intersection, we need to pass the pointer as a referece
{
    _interOut = out;
    out->addStreet(shared_from_this()); // share the current street to the intersection
}

