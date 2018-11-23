//motor definitions for full robot

#ifndef _MOTORDEFS_H_
#define _MOTORDEFS_H_

namespace okapi{
    class Motor;
}

using namespace okapi;

class MotorDefs {
    public:
        Motor *left_mtr_f;
        Motor *left_mtr_b;
        Motor *left_mtr_m;
        Motor *right_mtr_f;
        Motor *right_mtr_b;
        Motor *right_mtr_m;
        Motor *catapult_mtr;
        Motor *intake_mtr;

        MotorDefs();
        ~MotorDefs();
};


#endif //_MOTORDEFS_H_