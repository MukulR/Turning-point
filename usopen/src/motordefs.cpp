#include "motordefs.hpp"



MotorDefs::MotorDefs() {
    left_mtr_f = new pros::Motor(15, pros::E_MOTOR_GEARSET_18, false);
    right_mtr_f = new pros::Motor(12, pros::E_MOTOR_GEARSET_18, true);
    left_mtr_b = new pros::Motor(8, pros::E_MOTOR_GEARSET_18, false);
    right_mtr_b = new pros::Motor(16, pros::E_MOTOR_GEARSET_18, true);
    middle_mtr = new pros::Motor(20, pros::E_MOTOR_GEARSET_18, false);
    catapult_mtr = new pros::Motor(10, pros::E_MOTOR_GEARSET_36, false);
    intake_mtr = new pros::Motor(1, pros::E_MOTOR_GEARSET_06, false);
    flipper_mtr = new pros::Motor(4, pros::E_MOTOR_GEARSET_18, true);
}

MotorDefs::~MotorDefs() { 
    delete left_mtr_b;
    delete left_mtr_f;
    delete right_mtr_f;
    delete right_mtr_b;
    delete middle_mtr;
    delete catapult_mtr;
    delete intake_mtr;
    delete flipper_mtr;
} 