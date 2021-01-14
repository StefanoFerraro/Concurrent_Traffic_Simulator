#include <iostream>
#include <thread>
#include <future>
#include <algorithm>

#include "TrafficLight.h"

template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock, [this] {return !_deque.empty();} );

    T message = std::move(_deque.back());
    _deque.pop_back(); // retrive message from the queue and pop it

    return message;
}

template <typename T>
void MessageQueue<T>::send(T &&message)
{
    std::lock_guard<std::mutex> uLock(_mutex);
    _deque.clear(); // only one element at a time is allowed in the queue (the latest phase of the traffic light)
    _deque.push_back(std::move(message));
    _cond.notify_one();
}

TrafficLight::TrafficLight()
{   
    _type = ObjectType::objectTrafficLight;
    _currentPhase = trafficLightPhase::red;
    queue.send(std::move(_currentPhase));
}

void TrafficLight::waitForGreen()
{   
    while(true)
    {   
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        trafficLightPhase message = queue.receive();
 
        if(message == trafficLightPhase::green)
        {
            return;
        }
    }    
}

void TrafficLight::simulate()
{
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

void TrafficLight::cycleThroughPhases()
{
    std::chrono::time_point<std::chrono::system_clock> lastUpdate = std::chrono::system_clock::now();
    double time_limit = 4.0 + (float) rand()/(RAND_MAX/2); // output a random number between 4 and 6

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); //sleep for reducing CPU usage
        
        long deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        
        if(deltaTime > time_limit*1000) // After a specific time switch the traffic light
        {
            
            switch(_currentPhase)
            {
                case trafficLightPhase::green : 

                    _currentPhase = trafficLightPhase::yellow;

                    queue.send(std::move(_currentPhase));   // send yellow phase to the message queue
                    
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //sleep 1 sec in yellow mode
                    
                    _currentPhase = trafficLightPhase::red;
                break;

                case trafficLightPhase::red : 
                    _currentPhase = trafficLightPhase::green;
                break;   
            } 

            queue.send(std::move(_currentPhase));   // send new phase to the message queue

            lastUpdate = std::chrono::system_clock::now();
            time_limit = 4.0 + (float) rand()/(RAND_MAX/2);
        }

    }

}