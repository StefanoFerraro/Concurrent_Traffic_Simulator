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
        std::deque<T> _deque;
        std::condition_variable _cond;
        std::mutex _mutex;
};

class TrafficLight : public TrafficObject
{
    public:
        TrafficLight();
        void waitForGreen();
        void simulate();
        trafficLightPhase getCurrentPhase() {return _currentPhase;}

    private:
        void cycleThroughPhases(); 
        trafficLightPhase _currentPhase;
        std::shared_ptr<MessageQueue<trafficLightPhase>> queue;
};

#endif