#ifndef _ROBOT_DRIVER_H_
#define _ROBOT_DRIVER_H_

class MotorDefs;

class RobotDriver {
    private:
        MotorDefs *mtrDefs;
    public:
        RobotDriver(MotorDefs *motorDefs);
        ~RobotDriver();
        void driveRobot(double degrees, std::int32_t velocity);
        void driveWithCoast(int time, int power);
        void turnRobot(int unscaledDegs, bool left);
        void smoothDrive(int degrees, int power, int direction);
    private:
        void waitForDriveCompletion();
        void waitForTurnCompletion();
};


#endif //_ROBOT_DRIVER_H_