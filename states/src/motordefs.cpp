#include "motordefs.hpp"
#include "okapi/api.hpp"

using namespace okapi;

MotorDefs::MotorDefs() {
    left_mtr_f = new Motor(10, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); //default motor color
    right_mtr_f = new Motor(20, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); //default motor color
    left_mtr_b = new Motor(5, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); //default motor color
    right_mtr_b = new Motor(1, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); //default motor color
    right_mtr_m = new Motor(15, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); //default motor color
    left_mtr_m = new Motor(7, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); //default motor color
    catapult_mtr = new Motor(18, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees); 
    intake_mtr = new Motor(12, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
}

MotorDefs::~MotorDefs() { 
    delete left_mtr_b;
    delete left_mtr_f;
    delete right_mtr_f;
    delete right_mtr_b;
    delete right_mtr_m;
    delete left_mtr_m;
    delete catapult_mtr;
    delete intake_mtr;
} 