#ifndef _UOM_H_
#define _UOM_H_

class RobotDriver;
class MotorDefs;
class Commons;

namespace pros{
    class ADIGyro;
}

class UOM{
    private:
        MotorDefs *mtrDefs;
        RobotDriver *robotDriver;
        bool redAlliance;
        Commons *commonAutons;
        pros::ADIGyro *gyro;
    public:
        UOM(MotorDefs *mtrDefs, RobotDriver *rd, bool ra, Commons *ca, pros::ADIGyro *gyro);
        ~UOM();
        void runAuton();
        void getBallFromUnderCapAndAlignAgainstFence();
};

#endif //_UOM_H_