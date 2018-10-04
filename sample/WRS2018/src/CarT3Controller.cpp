/**
   @author Shin'ichiro Nakaoka
*/

#include <cnoid/SimpleController>
#include <cnoid/Device>
#include <cnoid/ForceSensor>
#include <cnoid/MarkerDevice>
#include "SpreaderController.h"

using namespace std;
using namespace cnoid;

class CarT3Controller : public SimpleController
{
    struct SpreaderTarget {
        Link* link;
        ForceSensorPtr forceSensor;
        DevicePtr breakableJoint;
        double timeToBreak;
        int forceIteration;
        double forceDuration;
        double force;
        double markerDuration;
        string sensorName;
        string jointName;
        SpreaderTarget(const char* sensorName, const char* jointName)
            : sensorName(sensorName), jointName(jointName) {
            timeToBreak = 0.0;
            forceIteration = 0;
            markerDuration = 0.0;
        }
    };
    vector<SpreaderTarget> targets;
    SimpleControllerIO* io;
    double timeStep;
    SpreaderController* spreaderController;
    Link* door;
    MarkerDevice* marker;
    
public:

    virtual bool initialize(SimpleControllerIO* io) override
    {
        this->io = io;
        timeStep = io->timeStep();
        
        targets = {
            { "FR_DOOR_UPPER_HINGE_FORCE_SENSOR", "FR_DOOR_UPPER_HINGE_CONSTRAINT" },
            { "FR_DOOR_LOWER_HINGE_FORCE_SENSOR", "FR_DOOR_LOWER_HINGE_CONSTRAINT" },
            { "FR_DOOR_LOCK_FORCE_SENSOR",  "FR_DOOR_LOCK_CONSTRAINT"  }
        };

        auto body = io->body();
        auto iter = targets.begin();
        while(iter != targets.end()){
            auto& target = *iter;
            target.forceSensor = body->findDevice<ForceSensor>(target.sensorName);
            target.breakableJoint = body->findDevice(target.jointName);
            if(target.forceSensor && target.breakableJoint){
                target.link = target.forceSensor->link();
                io->enableInput(target.forceSensor);
                ++iter;
            } else {
                iter = targets.erase(iter);
            }
        }

        io->os() << io->controllerName() << ": ";
        if(targets.empty()){
            io->os() << "No spreader target was found." << endl;
            return false;
        }
        io->os() << targets.size() << " spreader targets have been found." << endl;

        door = body->link("FR_DOOR");
        io->enableOutput(door, LINK_FORCE);

        marker = body->findDevice<MarkerDevice>("DestructionMarker");
        if(!marker){
            io->os() << "DestructionMarker was not found." << endl;
            return false;
        }
        io->enableInput(marker);
        marker->on(false);
        marker->notifyStateChange();

        return !targets.empty();
    }

    virtual bool start() override
    {
        spreaderController = SpreaderController::instance();
        return true;
    }

    virtual bool control() override
    {
        bool doRequestToSpread = false;
        
        for(auto& target : targets){
            target.link->F_ext().setZero();
            double fy = target.forceSensor->f().y();
            if(fy > 100.0){
                doRequestToSpread = true;
                if(target.breakableJoint->on()){
                    target.timeToBreak += timeStep;
                    //io->os() << target.forceSensor->name() << "'s time counter is " << target.timeToBreak << endl;
                    if(target.timeToBreak > 1.5){
                        target.breakableJoint->on(false);
                        target.breakableJoint->notifyStateChange();

                        /*
                        target.forceIteration = 1;
                        target.forceDuration = 0.1;
                        target.force = -10.0;
                        */

                        target.markerDuration = 0.8;
                        marker->setMarkerSize(0.001);
                        marker->on(true);
                        marker->notifyStateChange();
                        marker->setOffsetTranslation(target.link->offsetTranslation());
                    }
                }
            }
            /*
            if(target.forceIteration > 0){
                //io->os() << door->name() << ": force applied" << endl;
                Vector3 f = door->T() * Vector3(0.0, target.force, 0.0);
                door->addExternalForce(f, Vector3(-0.72, -0.02, 0.3));
                target.forceDuration -= timeStep;
                if(target.forceDuration <= 0.0){
                    target.forceDuration = 0.1;
                    target.force = -target.force;
                    --target.forceIteration;
                }
            }
            */
            if(target.markerDuration > 0.0){
                marker->setMarkerSize(marker->markerSize() + 0.003);
                target.markerDuration -= timeStep;
                if(target.markerDuration <= 0.0){
                    marker->on(false);
                }
                marker->notifyStateChange();
            }
        }

        if(spreaderController){
            spreaderController->requestToSpread(doRequestToSpread);
        }
        
        return targets.empty();
    }
};

CNOID_IMPLEMENT_SIMPLE_CONTROLLER_FACTORY(CarT3Controller)