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

void initPIDVals(MotorDefs *mtrDefs){
	pros::motor_pid_s_t pid = pros::Motor::convert_pid(KF, KP, KI, KD);
	mtrDefs->left_mtr_f->set_pos_pid(pid);
	mtrDefs->left_mtr_b->set_pos_pid(pid);
	mtrDefs->right_mtr_f->set_pos_pid(pid);
	mtrDefs->right_mtr_b->set_pos_pid(pid);
	mtrDefs->middle_mtr->set_pos_pid(pid);
}

void autonomous() {
	MotorDefs mtrDefs;
	initPIDVals(&mtrDefs);
	switch (autonSelected) {
		case 0:
			pros::lcd::set_text(2, "Red Auton Running!");
			//frontAuton(&mtrDefs, true /* red alliance */);
			break;
		case 1:
			pros::lcd::set_text(2, "Blue Auton Running!");
			//frontAuton(&mtrDefs, false /* blue alliance */);
			break;
		case 2:
			pros::lcd::set_text(2, "Red Back With Park Auton Running!");
			//backAuton(&mtrDefs, true /* red alliance */);
			break;
		case 3:
			pros::lcd::set_text(2, "Blue Back With Park Auton Running!");
			//backAuton(&mtrDefs, false /* blue alliance */);
			break;
		default:
			pros::lcd::set_text(2, "no auton");
			//noAuton();
			break;
	}
}
