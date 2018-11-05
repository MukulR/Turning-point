//motor definitions for full robot

#ifndef _MOTORDEFS_H_
#define _MOTORDEFS_H_

class MotorDefs {
    public:
        /*
        pros::Motor *left_mtr_f = NULL;
        pros::Motor *left_mtr_b = NULL;
        pros::Motor *right_mtr_f = NULL;
        pros::Motor *right_mtr_b = NULL;
        */
        pros::Motor *catapult_mtr = NULL;
        pros::Motor *intake_mtr = NULL;

        MotorDefs() {
            /*  
            left_mtr_f = new pros::Motor(1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
            left_mtr_b = new pros::Motor(3, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
            right_mtr_f = new pros::Motor(2, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
            right_mtr_b = new pros::Motor(4, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
            */
            catapult_mtr = new pros::Motor(5, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);
            intake_mtr = new pros::Motor(6, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
        }
        ~MotorDefs() {
            /*
            delete left_mtr_b;
            delete left_mtr_f;
            delete right_mtr_f;
            delete right_mtr_b;
            */
            delete catapult_mtr;
            delete intake_mtr;
        }
};


#endif //_MOTORDEFS_H_