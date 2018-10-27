#ifndef _MOTORDEFS_H_
#define _MOTORDEFS_H_

// Forward declaration for okapi::Motor type.
namespace okapi {
    class Motor;
};

using namespace okapi;

class MotorDefs {
    public:
        Motor *left_mtr_f;
        Motor *left_mtr_b;
        Motor *right_mtr_f;
        Motor *right_mtr_b;
        Motor *catapult_mtr;
        Motor *intake_mtr;
        Motor *brake_mtr;

        MotorDefs();
        ~MotorDefs();

    private:
        static MotorDefs *mtrDefs;
    public:
        static MotorDefs *getMotorDefs() {
            return mtrDefs;
        }
};

#endif //_MOTORDEFS_H_