#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Graphics.h"
#include "Intersection.h"
#include "TrafficLight.h"

void Graphics::simulate()
{
    this->loadBackgroundImg();
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); //reduce cpu usage, increase sleep time for lower the refresh rate
        this->drawTrafficObjects();
    }
}

void Graphics::loadBackgroundImg()
{
    _windowName = "Traffic Simulator";
    cv::namedWindow(_windowName, 0);
    // std::vector<int> screen_res {1920, 1080};
    // int window_width

    cv::resizeWindow(_windowName, 1920, 1080);

    // load image and create copy to be used for semi-transparent overlay
    cv::Mat background = cv::imread(_bgFilename);
    _images.push_back(background);         // first element is the original background
    _images.push_back(background.clone()); // second element will be the transparent overlay
    _images.push_back(background.clone()); // third element will be the result image for display
}

void Graphics::drawTrafficObjects()
{
    _images.at(1) = _images.at(0).clone();
    _images.at(2) = _images.at(0).clone();
    
    // create overlay from all traffic objects
    for (auto it : _trafficObjects)
    {
        double posx, posy;
        it->getPosition(posx, posy); //position of the object (Vehicle/Intersection)

        if (it->getType() == ObjectType::objectIntersection) //if the object is an Intersection
        {
            // cast object type from TrafficObject to Intersection, needed for using the method 'trafficLightIsGreen'
            std::shared_ptr<Intersection> intersection = std::dynamic_pointer_cast<Intersection>(it);

            cv::Scalar trafficLightColor;

            // set color according to traffic light and draw the intersection as a circle
            switch(intersection->trafficLightColor())
            {
                case trafficLightPhase::green : trafficLightColor = cv::Scalar(0, 255, 0); break;

                case trafficLightPhase::yellow : trafficLightColor = cv::Scalar(20, 255, 255); break;

                case trafficLightPhase::red : trafficLightColor = cv::Scalar(0, 0, 255); break;
            }
            
            //cv::Scalar trafficLightColor = intersection->trafficLightColor() == trafficLightPhase:: ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 0, 255);
            cv::circle(_images.at(1), cv::Point2d(posx, posy), 25, trafficLightColor, -1);

        }
        else if (it->getType() == ObjectType::objectVehicle) //if the object is a Vehicle
        {
            cv::RNG rng(it->getID()); //random number generator seeded with the same id gives always the same number (used as reference for color pick)
            int b = rng.uniform(0, 255);
            int g = rng.uniform(0, 255);
            int r = sqrt(255*255 - g*g - r*r); // ensure that length of color vector is always 255
            cv::Scalar vehicleColor = cv::Scalar(b,g,r); // generate a scalar vector to be filled into the circle genrator
            cv::circle(_images.at(1), cv::Point2d(posx, posy), 50, vehicleColor, -1);
        }
    }

    float opacity = 0.85;
    cv::addWeighted(_images.at(1), opacity, _images.at(0), 1.0 - opacity, 0, _images.at(2));

    // display background and overlay image
    cv::imshow(_windowName, _images.at(2));
    cv::waitKey(33); 
}