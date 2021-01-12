#ifndef STREET_H
#define STREET_H

#include "TrafficObject.h"

class Intersection; //forward declaration needed for using the following in the current definition refer to: @@@

class Street : public TrafficObject, public std::enable_shared_from_this<Street> //needed for sharing the current thread
{
    public:
        // constructor
        Street();

        double getLength() {return _length;}
        void setInIntersection(std::shared_ptr<Intersection> &in);
        void setOutIntersection(std::shared_ptr<Intersection> &out);
        std::shared_ptr<Intersection> getInIntersection() {return _interIn;}
        std::shared_ptr<Intersection> getOutIntersection() {return _interOut;}

    private:
        double _length;
        std::shared_ptr<Intersection> _interIn, _interOut;      //@@@
};

#endif