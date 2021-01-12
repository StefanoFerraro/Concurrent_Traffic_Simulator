#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include "TrafficObject.h"

enum trafficLightPhase
{
    red,
    yellow,
    green
};

class TrafficLight : public TrafficObject
{
    public:
        TrafficLight();
        // void waitForGreen();
        void simulate();
        trafficLightPhase getCurrentPhase() {return _currentPhase;}

    private:
        void cycleThroughPhases(); 
        trafficLightPhase _currentPhase;
};

#endif