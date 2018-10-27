#include "motordefs.hpp"
#include "okapi/api.hpp"

MotorDefs *MotorDefs::mtrDefs = new MotorDefs();

using namespace okapi;

MotorDefs::MotorDefs() {
    left_mtr_f = new Motor(1, false /* Not Reversed */, AbstractMotor::gearset::green);
    right_mtr_f = new Motor(2, true /* Reversed */, AbstractMotor::gearset::green);
    left_mtr_b = new Motor(3, false /* Not Reversed */, AbstractMotor::gearset::green);
    right_mtr_b = new Motor(4, true /* Reversed */, AbstractMotor::gearset::green);

    catapult_mtr = new Motor(5, false /* Not Reversed */, AbstractMotor::gearset::red);
    intake_mtr = new Motor(6, false /* Not Reversed */, AbstractMotor::gearset::green);
}

MotorDefs::~MotorDefs() {
    delete left_mtr_b;
    delete left_mtr_f;
    delete right_mtr_f;
    delete right_mtr_b;
    delete catapult_mtr;
    delete intake_mtr;
}