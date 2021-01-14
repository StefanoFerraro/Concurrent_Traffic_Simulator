#include <iostream>
#include <cmath>
#include "Intersection.h"
#include "Street.h"

Street::Street()
{
    _type = ObjectType::objectStreet;
    _length = 0;
}

void Street::setStreet(std::shared_ptr<Intersection> &in, std::shared_ptr<Intersection> &out)
{

    this->setInIntersection(in);
    this->setOutIntersection(out);

    double x1, x2, y1, y2;
    in->getPosition(x1, y1);
    out->getPosition(x2, y2);

    _length = std::sqrt(std::pow(x2 - x1,2) + std::pow(y2 - y1,2)); // set street legth based on the position of in/out intersections
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

