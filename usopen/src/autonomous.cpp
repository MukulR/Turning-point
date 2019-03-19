#include "autonselection.h"
#include "motordefs.hpp"

//PID constants
#define KF 0
#define KP 1.0f
#define KI 0.001f
#define KD 0.1f

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */


void frontAuton(MotorDefs *mtrDefs, bool redAlliance);
void backAuton(MotorDefs *mtrDefs, bool redAlliance);
void noAuton();

void initPIDVals(MotorDefs *mtrDefs){
	pros::motor_pid_s_t pid = pros::Motor::convert_pid(KF, KP, KI, KD);
	mtrDefs->left_mtr_f->set_pos_pid(pid);
	mtrDefs->left_mtr_b->set_pos_pid(pid);
	mtrDefs->right_mtr_f->set_pos_pid(pid);
	mtrDefs->right_mtr_b->set_pos_pid(pid);
	mtrDefs->middle_mtr->set_pos_pid(pid);

	mtrDefs->left_mtr_f->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	mtrDefs->left_mtr_b->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	mtrDefs->right_mtr_f->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	mtrDefs->right_mtr_b->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	mtrDefs->middle_mtr->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);

}

void driveRobot(MotorDefs *mtrDefs, double degrees, std::int32_t velocity){
	mtrDefs->left_mtr_f->move_relative(degrees, velocity);
	mtrDefs->left_mtr_b->move_relative(degrees, velocity);
	mtrDefs->right_mtr_f->move_relative(degrees, velocity);
	mtrDefs->right_mtr_b->move_relative(degrees, velocity);
	mtrDefs->middle_mtr->move_relative(degrees, velocity);
}

void driveWithCoast(MotorDefs *mtrDefs, int time, int power){
	mtrDefs->left_mtr_f->move(power);
	mtrDefs->left_mtr_b->move(power);
	mtrDefs->right_mtr_f->move(power);
	mtrDefs->right_mtr_b->move(power);
	mtrDefs->middle_mtr->move(power);
	pros::Task::delay(time);
	mtrDefs->left_mtr_f->move(0);
	mtrDefs->left_mtr_b->move(0);
	mtrDefs->right_mtr_f->move(0);
	mtrDefs->right_mtr_b->move(0);
	mtrDefs->middle_mtr->move(0);
}

void turnRobot(MotorDefs *mtrDefs, int degrees, bool left){
	mtrDefs->left_mtr_f->move_relative(degrees, 65);	
	mtrDefs->left_mtr_b->move_relative(degrees, 65);
	mtrDefs->right_mtr_f->move_relative(-degrees, 65);
	mtrDefs->right_mtr_b->move_relative(-degrees, 65);
}

void smoothDrive(MotorDefs *mtrDefs, int degrees, int power){
	driveWithCoast(mtrDefs, 200, 10);
	driveWithCoast(mtrDefs, 200, 25);
	driveWithCoast(mtrDefs, 200, 50);
	driveRobot(mtrDefs, degrees, power);
}

void autonomous() {
	MotorDefs mtrDefs;
	initPIDVals(&mtrDefs);
	switch (autonSelected) {
		case 0:
			pros::lcd::set_text(2, "Red Auton Running!");
			frontAuton(&mtrDefs, true /* red alliance */);
			break;
		case 1:
			pros::lcd::set_text(2, "Blue Auton Running!");
			frontAuton(&mtrDefs, false /* blue alliance */);
			break;
		case 2:
			pros::lcd::set_text(2, "Red Back With Park Auton Running!");
			backAuton(&mtrDefs, true /* red alliance */);
			break;
		case 3:
			pros::lcd::set_text(2, "Blue Back With Park Auton Running!");
			backAuton(&mtrDefs, false /* blue alliance */);
			break;
		default:
			pros::lcd::set_text(2, "no auton");
			noAuton();
			break;
	}
}

void getPlatformBall(MotorDefs *mtrDefs, bool redAlliance){
	smoothDrive(mtrDefs, 450, 80);
	if(redAlliance){
		turnRobot(mtrDefs, 45, false);
	} else {
		turnRobot(mtrDefs, 45, true);
	}
}

void frontAuton(MotorDefs *mtrDefs, bool redAlliance){
	getPlatformBall(mtrDefs, redAlliance);
}

void backAuton(MotorDefs *mtrDefs, bool redAlliance){

}


void noAuton(){}