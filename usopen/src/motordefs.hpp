//motor definitions for full robot
#include "main.h"
#ifndef _MOTORDEFS_H_
#define _MOTORDEFS_H_



class MotorDefs {
    public:
        pros::Motor *left_mtr_f;
        pros::Motor *left_mtr_b;
        pros::Motor *middle_mtr;
        pros::Motor *right_mtr_f;
        pros::Motor *right_mtr_b;
        pros::Motor *catapult_mtr;
        pros::Motor *intake_mtr;

        MotorDefs();
        ~MotorDefs();
};


#endif //_MOTORDEFS_H_