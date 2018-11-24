//#include "main.h"
#include "okapi/api.hpp"
#include "autonselection.h"
#include "motordefs.hpp"

using namespace okapi;

const float TURN_SCALE_FACTOR = 4.1;

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
void frontAuton(MotorDefs *mtrDefs, bool redAlliance, bool park);
void backAuton(MotorDefs *mtrDefs, bool redAlliance, bool park);
void noAuton();

void autonomous() {
	MotorDefs mtrDefs;
	switch (autonSelected) {
		case 0:
			pros::lcd::set_text(2, "Red Auton Running!");
			frontAuton(&mtrDefs, true /* red alliance */, false /* no parking */);
			break;
		case 1:
			pros::lcd::set_text(2, "Red With Park Auton Running!");
			frontAuton(&mtrDefs, true /* red alliance */, true /* parking */);
			break;
		case 2:
			pros::lcd::set_text(2, "Blue Auton Running!");
			frontAuton(&mtrDefs, false /* blue alliance */, false /* no parking */);
			break;
		case 3:
			pros::lcd::set_text(2, "Blue With Park Auton Running!");
			frontAuton(&mtrDefs, false /* blue alliance */, true /* parking */);
			break;
		case 4:
			pros::lcd::set_text(2, "Back Auton Running!");
			backAuton(&mtrDefs, false /* blue alliance */, false /* no parking */);
			break;
		case 5:
			pros::lcd::set_text(2, "Back With Park Auton Running!");
			backAuton(&mtrDefs, false /* blue alliance */, true /* parking */);
			break;
		default:
			pros::lcd::set_text(2, "no auton");
			noAuton();
			break;
	}
}

void driveRobot(MotorDefs *mtrDefs, int power, int travelTime){
	mtrDefs->left_mtr_f->move(power);
	mtrDefs->left_mtr_b->move(power);
	mtrDefs->right_mtr_f->move(power);
	mtrDefs->right_mtr_b->move(power);
	mtrDefs->right_mtr_m->move(power);
	mtrDefs->left_mtr_m->move(power);
	pros::Task::delay(travelTime);
	mtrDefs->left_mtr_f->move(0);
	mtrDefs->right_mtr_f->move(0);
	mtrDefs->left_mtr_b->move(0);
	mtrDefs->right_mtr_b->move(0);
	mtrDefs->right_mtr_m->move(0);
	mtrDefs->left_mtr_m->move(0);
}

void turnDegrees(MotorDefs *mtrDefs, int degrees, bool left){
	if(left){
		mtrDefs->left_mtr_f->move(-65);
		mtrDefs->left_mtr_b->move(-65);
		mtrDefs->left_mtr_m->move(-65);
		mtrDefs->right_mtr_f->move(65);
		mtrDefs->right_mtr_b->move(65);
		mtrDefs->right_mtr_m->move(65);
	} else {
		mtrDefs->left_mtr_f->move(65);
		mtrDefs->left_mtr_b->move(65);
		mtrDefs->left_mtr_m->move(65);
		mtrDefs->right_mtr_f->move(-65);
		mtrDefs->right_mtr_b->move(-65);
		mtrDefs->right_mtr_m->move(-65);
	}
	pros::Task::delay(TURN_SCALE_FACTOR * degrees);
	mtrDefs->left_mtr_b->move(0);
	mtrDefs->left_mtr_f->move(0);
	mtrDefs->left_mtr_m->move(0);
	mtrDefs->right_mtr_f->move(0);
	mtrDefs->right_mtr_b->move(0);
	mtrDefs->right_mtr_m->move(0);
}

void pickupAnotherBallAndComeBack(MotorDefs *mtrDefs){
	pros::Task::delay(200);
	driveRobot(mtrDefs, 80, 1200);
	mtrDefs->intake_mtr->move(127);
	pros::Task::delay(300);
	// move back with ball and preload ball towards fence
	driveRobot(mtrDefs, -80, 1300);
}

void alignAndShoot(MotorDefs *mtrDefs, bool redAlliance){
	pros::Task::delay(500);
	if(redAlliance == true){
		driveRobot(mtrDefs, 70, 275);
	} else {
		driveRobot(mtrDefs, 70, 100);
	}
	pros::Task::delay(500);
	//turn to face flags
	if(redAlliance){
		turnDegrees(mtrDefs, 84, true /* turn left */);
	} else {
		turnDegrees(mtrDefs, 90, false /* turn right */);
	}
	pros::Task::delay(1000);
	mtrDefs->catapult_mtr->move_relative(415, 127);
	mtrDefs->intake_mtr->move(0);
}

void flipBottomFlagAndBackToTile(MotorDefs *mtrDefs, bool redAlliance){
	if (redAlliance) {
		turnDegrees(mtrDefs, 15, true /* turn left */);
	} else {
		turnDegrees(mtrDefs, 2, false /* turn right */);
	}
	driveRobot(mtrDefs, 100, 1000);
	pros::Task::delay(300);
	driveRobot(mtrDefs, -100, 750);
	pros::Task::delay(200);
	
	if (redAlliance) {
		turnDegrees(mtrDefs, 83, false /* turn right */);
	} else {
		turnDegrees(mtrDefs, 82, true /* turn left */);
	}
	pros::Task::delay(200);
	driveRobot(mtrDefs, -100, 750);
	pros::Task::delay(200);
}

void flipCap(MotorDefs *mtrDefs, bool redAlliance){
	mtrDefs->intake_mtr->move(-127);
	pros::Task::delay(1000); 
	if(redAlliance){
		turnDegrees(mtrDefs, 100, true /* turn left */);
	} else {
		turnDegrees(mtrDefs, 90, false /* turn right */);
	}
	
	driveRobot(mtrDefs, 60, 1500);
	mtrDefs->intake_mtr->move(0);
	pros::Task::delay(200);
	driveRobot(mtrDefs, -60, 700);
}

void flipMidLowerFlag(MotorDefs *mtrDefs, bool redAlliance){
	driveRobot(mtrDefs, 90, 900);
	pros::Task::delay(500);
	if(redAlliance){
		turnDegrees(mtrDefs, 85, true /* turn left */);
	} else {
		turnDegrees(mtrDefs, 75, false /* turn right */);
	}
	pros::Task::delay(200);
	driveRobot(mtrDefs, 100, 1500);
	pros::Task::delay(200);
	driveRobot(mtrDefs, -100, 450);
	pros::Task::delay(500);
}

void parkOnPlatform(MotorDefs *mtrDefs, bool redAlliance){

}


void frontAuton(MotorDefs *mtrDefs, bool redAlliance, bool park){
	pickupAnotherBallAndComeBack(mtrDefs);
	alignAndShoot(mtrDefs, redAlliance);
	pros::Task::delay(500);
	flipBottomFlagAndBackToTile(mtrDefs, redAlliance);

	if (park) {
		parkOnPlatform(mtrDefs, redAlliance);
	} else {
		flipMidLowerFlag(mtrDefs, redAlliance);
	 	flipCap(mtrDefs, redAlliance);
	}
}


void backAuton(MotorDefs *mtrDefs, bool redAlliance, bool park){
	pros::Task::delay(12000);
	mtrDefs->catapult_mtr->move(127);
	pros::Task::delay(500);
	mtrDefs->catapult_mtr->move(0);
}

void noAuton(){
}