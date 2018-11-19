#include "motordefs.hpp"
#include "okapi/api.hpp"

using namespace okapi;

MotorDefs::MotorDefs() {
    left_mtr_f = new Motor(1, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); //default motor
    right_mtr_f = new Motor(2, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); //default motor
    left_mtr_b = new Motor(3, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); //default motor
    right_mtr_b = new Motor(4, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); //default motor
    catapult_mtr = new Motor(5, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees); 
    intake_mtr = new Motor(6, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
}

MotorDefs::~MotorDefs() { 
    delete left_mtr_b;
    delete left_mtr_f;
    delete right_mtr_f;
    delete right_mtr_b;
    delete catapult_mtr;
    delete intake_mtr;
} 