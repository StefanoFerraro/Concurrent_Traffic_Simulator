#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <deque>
#include <mutex>
#include <condition_variable>
#include "TrafficObject.h"

enum trafficLightPhase
{
    red,
    yellow,
    green
};

// Implementation of a general message queue (any type of message)
template <class T> // same to template <typename T> used to define a general type
class MessageQueue
{
    public:
        void send(T &&message);
        T receive();

    private: 
        std::mutex _mutex;
        std::condition_variable _cond;
        std::deque<T> _deque;
        
        
};

class TrafficLight : public TrafficObject, public std::enable_shared_from_this<TrafficLight>
{
    public:
        TrafficLight();
        void waitForGreen();
        void simulate();
        trafficLightPhase getCurrentPhase() {return _currentPhase;}

    private:
        void cycleThroughPhases(); 
        trafficLightPhase _currentPhase;
        MessageQueue<trafficLightPhase> queue;
};

#endif