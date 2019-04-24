#include "motordefs.hpp"

//PID constants
#define KF 0
#define KP 1.0f
#define KI 0.001f
#define KD 0.1f


MotorDefs::MotorDefs() {
    /*
    left_mtr_f = new pros::Motor(15, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
    right_mtr_f = new pros::Motor(12, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
    left_mtr_b = new pros::Motor(8, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
    right_mtr_b = new pros::Motor(16, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
    middle_mtr = new pros::Motor(20, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
    catapult_mtr = new pros::Motor(10, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);
    intake_mtr = new pros::Motor(1, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
    flipper_mtr = new pros::Motor(4, pros::E_MOTOR_GEARSET_36, true, pros::E_MOTOR_ENCODER_DEGREES);
    */
    left_mtr_f = new pros::Motor(15, pros::E_MOTOR_GEARSET_18, false);
    right_mtr_f = new pros::Motor(12, pros::E_MOTOR_GEARSET_18, true);
    left_mtr_b = new pros::Motor(8, pros::E_MOTOR_GEARSET_18, false);
    right_mtr_b = new pros::Motor(16, pros::E_MOTOR_GEARSET_18, true);
    middle_mtr = new pros::Motor(20, pros::E_MOTOR_GEARSET_18, false);
    catapult_mtr = new pros::Motor(10, pros::E_MOTOR_GEARSET_36, false);
    intake_mtr = new pros::Motor(1, pros::E_MOTOR_GEARSET_06, false);
    flipper_mtr = new pros::Motor(4, pros::E_MOTOR_GEARSET_36, true);
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

void MotorDefs::initPIDVals(){
    pros::motor_pid_s_t pid = pros::Motor::convert_pid(KF, KP, KI, KD);
	left_mtr_f->set_pos_pid(pid);
	left_mtr_b->set_pos_pid(pid);
	right_mtr_f->set_pos_pid(pid);
	right_mtr_b->set_pos_pid(pid);
	middle_mtr->set_pos_pid(pid);

    left_mtr_f->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	left_mtr_b->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	right_mtr_f->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	right_mtr_b->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	middle_mtr->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
}
