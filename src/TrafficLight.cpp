#include <iostream>
#include <thread>

#include "TrafficLight.h"

template <typename T>
void MessageQueue<T>::send(T &&message)
{
    std::lock_guard<std::mutex> uLock(_mutex);
    
    _deque.push_back(std::move(message));
    _cond.notify_one();
}

template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock, [this] {return !_deque.empty();} );

    T message = std::move(_deque.front());
    _deque.pop_front();

    return message;
}

TrafficLight::TrafficLight()
{   
    _type = ObjectType::objectTrafficLight;
    _currentPhase = trafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    while(true)
    {
        trafficLightPhase message = queue->receive();

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

    std::shared_ptr<MessageQueue<trafficLightPhase>> queue = std::make_shared<MessageQueue<trafficLightPhase>>(); // message init definition

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
                    queue->send(std::move(_currentPhase));
                    
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //sleep 1 sec in yellow mode
                    
                    _currentPhase = trafficLightPhase::red;
                    queue->send(std::move(_currentPhase));
                break;

                case trafficLightPhase::red : 
                    _currentPhase = trafficLightPhase::green;
                    queue->send(std::move(_currentPhase));
                break;   
            } 

            //_currentPhase = _currentPhase == trafficLightPhase::red ? trafficLightPhase::green : trafficLightPhase::red;

            lastUpdate = std::chrono::system_clock::now();
            time_limit = 4.0 + (float) rand()/(RAND_MAX/2);
        }

    }
}