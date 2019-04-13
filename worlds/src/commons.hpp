#ifndef _COMMONS_H_
#define _COMMONS_H_

class MotorDefs;
class RobotDriver;

class Commons {
    public:
        MotorDefs *mtrDefs;
        RobotDriver *robotDriver;
        bool redAlliance;

        Commons(MotorDefs *md, RobotDriver *rd, bool ra);
        ~Commons();
    public:
        static void catapultLoad(void *param);
        
        void flipperMove(int potValue, int power, int direction);
        void shootCatapult();
        void pickupBallsFromCapFlipAndShoot();
        void alignAndShootOurFlags();
        void toggleLowFlag();
};
#endif //_COMMONS_H_