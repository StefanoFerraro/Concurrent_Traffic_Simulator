#ifndef TRAFFICOBJECT_H
#define TRAFFICOBJECT_H

#include <thread>
#include <vector>
#include <mutex>

enum ObjectType
{
    noObject,
    objectIntersection,
    objectStreet,
    objectVehicle
}; 

class TrafficObject
{
    public:

        //constructor/destructor
        TrafficObject();
        ~TrafficObject();

        //getter/setter
        void setPosition(double x, double y);
        void getPosition(double &x, double &y);
        int getID() {return _id;}
        ObjectType getType() {return _type;}

        virtual void simulate(){};  // virtual function needed for making the function polymorphic (could be any other function), 
                                    // this is required for the dynamic casting in the Graphic class

    protected:
        ObjectType _type;   //Identifu the type of traffic object
        int _id;    //unique id of the object
        double _posX, _posY;    //position of the object in px
        std::vector<std::thread> threads;   // vector of all the threads associated with the object
        static std::mutex _mtx;    //mutex shared by all traffic objects for protecting cout to console

    private:
        static int _idCnt;  // global variable for sharing the latest id used by another object
};

#endif
