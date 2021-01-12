#include <iostream>
#include <thread>

#include "TrafficLight.h"


TrafficLight::TrafficLight()
{   
    _type = ObjectType::objectTrafficLight;
    _currentPhase = trafficLightPhase::red;
}

// void TrafficLight::waitForGreen()
// {
    

// }

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
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); //sleep for reducing CPU usage
        
        long deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        
        if(deltaTime > time_limit*1000) // After a specific time switch the traffic light
        {
            
            switch(_currentPhase)
            {
                case trafficLightPhase::green : 
                    _currentPhase = trafficLightPhase::yellow;
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //sleep 1 sec in yellow mode
                    _currentPhase = trafficLightPhase::red;
                break;

                case trafficLightPhase::red : 
                    _currentPhase = trafficLightPhase::green;
                break;   
            } 

            //_currentPhase = _currentPhase == trafficLightPhase::red ? trafficLightPhase::green : trafficLightPhase::red;

            lastUpdate = std::chrono::system_clock::now();
            time_limit = 4.0 + (float) rand()/(RAND_MAX/2);
        }

    }
}