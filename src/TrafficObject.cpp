#include <iostream>
#include <algorithm>
#include "TrafficObject.h"


int TrafficObject::_idCnt = 0;  //init id to 0

std::mutex TrafficObject::_mtx; //static mutex definition, required for using the mutex in other contexts

// method for setting position of object
void TrafficObject::setPosition(double x, double y)
{
    _posX = x;
    _posY = y;
}

// method for getting position of object
void TrafficObject::getPosition(double &x, double &y)
{
    x = _posX;
    y = _posY;
}

// override of the constructor
TrafficObject::TrafficObject()
{
    _type = ObjectType::noObject;
    _id = _idCnt++;
}

// override  of the destructor
TrafficObject::~TrafficObject()
{   
    std::for_each(threads.begin(), threads.end(), [](std::thread &t){t.join();});
}



