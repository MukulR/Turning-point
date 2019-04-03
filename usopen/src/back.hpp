#ifndef _BACK_H_
#define _BACK_H_

class RobotDriver;
class MotorDefs;
class Commons;

class Back{
    private:
        MotorDefs *mtrDefs;
        RobotDriver *robotDriver;
        bool redAlliance;
        Commons *commonAutons;
    public:
        Back(MotorDefs *mtrDefs, RobotDriver *rd, bool ra, Commons *ca);
        ~Back();
        void runAuton();
};

#endif //_BACK_H_