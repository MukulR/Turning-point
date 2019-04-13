#ifndef _POM_H_
#define _POM_H_

class RobotDriver;
class MotorDefs;
class Commons;

class POM{
    private:
        MotorDefs *mtrDefs;
        RobotDriver *robotDriver;
        bool redAlliance;
        Commons *commonAutons;
    public:
        POM(MotorDefs *mtrDefs, RobotDriver *rd, bool ra, Commons *ca);
        ~POM();
        void runAuton();
        void getPlatformBallAndAlignAgainstFence();
};

#endif //_POM_H_