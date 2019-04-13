#ifndef _UOM_H_
#define _UOM_H_

class RobotDriver;
class MotorDefs;
class Commons;

class UOM{
    private:
        MotorDefs *mtrDefs;
        RobotDriver *robotDriver;
        bool redAlliance;
        Commons *commonAutons;
    public:
        UOM(MotorDefs *mtrDefs, RobotDriver *rd, bool ra, Commons *ca);
        ~UOM();
        void runAuton();
        void getBallFromUnderCapAndAlignAgainstFence();
};

#endif //_UOM_H_