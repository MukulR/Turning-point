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
void frontAuton(MotorDefs *mtrDefs, bool redAlliance);
void backAuton(MotorDefs *mtrDefs, bool redAlliance);
void noAuton();

void autonomous() {
	MotorDefs mtrDefs;
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
	driveRobot(mtrDefs, -100, 950);
}

void alignAndShoot(MotorDefs *mtrDefs, bool redAlliance){
	pros::Task::delay(500);	
	if(redAlliance == true){
		driveRobot(mtrDefs, 70, 225);
	} else {
		driveRobot(mtrDefs, 70, 100);
	}
	pros::Task::delay(500);
	//turn to face flags
	if(redAlliance){
		turnDegrees(mtrDefs, 83, true /* turn left */);
	} else {
		turnDegrees(mtrDefs, 83, false /* turn right */);
	}
	pros::Task::delay(400);
	mtrDefs->catapult_mtr->move_relative(415, 127);
	mtrDefs->intake_mtr->move(0);
	pros::Task::delay(1000);
}

void flipBottomFlagAndBackToTile(MotorDefs *mtrDefs, bool redAlliance){
	if (redAlliance) {
		turnDegrees(mtrDefs, 16, true /* turn left */); //was15
	} else {
		turnDegrees(mtrDefs, 3, false /* turn right */);
	}
	driveRobot(mtrDefs, 100, 1000);
	pros::Task::delay(300);
	driveRobot(mtrDefs, -100, 825);
	pros::Task::delay(200);
	
	if (redAlliance) {
		turnDegrees(mtrDefs, 83, false /* turn right */);
	} else {
		turnDegrees(mtrDefs, 82, true /* turn left */);
	}
	pros::Task::delay(200);
	driveRobot(mtrDefs, -100, 550);
	pros::Task::delay(200);
}

void flipCap(MotorDefs *mtrDefs, bool redAlliance){
	mtrDefs->intake_mtr->move(-127);
	pros::Task::delay(500); 
	if(redAlliance){
		turnDegrees(mtrDefs, 105, true /* turn left */);
	} else {
		turnDegrees(mtrDefs, 103, false /* turn right */);
	}
	
	driveRobot(mtrDefs, 70, 1250); //was 1300
	mtrDefs->intake_mtr->move(0);
	pros::Task::delay(200);
	driveRobot(mtrDefs, -60, 700);	
}

void flipMidLowerFlag(MotorDefs *mtrDefs, bool redAlliance){
	driveRobot(mtrDefs, 127, 700);
	pros::Task::delay(100);
	if(redAlliance){
		turnDegrees(mtrDefs, 93, true /* turn left */);
	} else {
		turnDegrees(mtrDefs, 95, false /* turn right */);
	}
	pros::Task::delay(100);
	driveRobot(mtrDefs, 100, 1300);
	pros::Task::delay(100);
	if(redAlliance){
		driveRobot(mtrDefs, -100, 200);
	} else {
		driveRobot(mtrDefs, -100, 175);
	}
	if(redAlliance){
		turnDegrees(mtrDefs, 93, false /* turn right */);
		driveRobot(mtrDefs, -50, 300);
	} else {
		turnDegrees(mtrDefs, 93, true /* turn left */);
		driveRobot(mtrDefs, -50, 300);
	}
	pros::Task::delay(100);
}

void loadBallFromBack(MotorDefs *mtrDefs, bool redAlliance) {
	// Move forward so that we clear the fence
	driveRobot(mtrDefs, 70, 25);
	pros::Task::delay(200);

	// turn to face the pick up ball under the cap
	if (redAlliance) {
		turnDegrees(mtrDefs, 52, false /* turn right */);
	} else {
		turnDegrees(mtrDefs, 50, true /* turn left */);
	}
	pros::Task::delay(200);
	driveRobot(mtrDefs, -70, 300);

	// Pick up ball from under the cap and come straight back to hit the fence
	// to ensure straight alignment.
	pickupAnotherBallAndComeBack(mtrDefs);
	pros::Task::delay(200);
	driveRobot(mtrDefs, -70, 200);
	pros::Task::delay(200);

	// Stop intake
	mtrDefs->intake_mtr->move(0);
}

void parkOnPlatform(MotorDefs *mtrDefs, bool redAlliance){
	// Move forward a bit so that we can clear the fence in preparation for turning.
	driveRobot(mtrDefs, 70, 200);
	pros::Task::delay(200);

	// Turn so that we go in front of the platform.
	if (redAlliance) {
		turnDegrees(mtrDefs, 90, true /* right left */ );
	} else {
		turnDegrees(mtrDefs, 95, false /* right turn */ );
	}
	pros::Task::delay(200);

	//Move forward so that the robot is within platform's range
	driveRobot(mtrDefs, 70, 920);
	pros::Task::delay(200);

	// Turn towards the fence so that back of the robot is facing the platform.
	if (redAlliance) {
		turnDegrees(mtrDefs, 97, true /*turn left */);
	} else {
		turnDegrees(mtrDefs, 95, false /*turn right */);
	}
	pros::Task::delay(200);

	// Move baack until the back wheels are aligned and touching the foot of the platform.
	driveRobot(mtrDefs, -50, 500);
	pros::Task::delay(500);

	// Climb up the platform.
	driveRobot(mtrDefs, -100, 1200);
	pros::Task::delay(200);
}


void frontAuton(MotorDefs *mtrDefs, bool redAlliance){
	pickupAnotherBallAndComeBack(mtrDefs);
	alignAndShoot(mtrDefs, redAlliance);
	flipBottomFlagAndBackToTile(mtrDefs, redAlliance);
	flipMidLowerFlag(mtrDefs, redAlliance);
 	//flipCap(mtrDefs, redAlliance);
}


void backAuton(MotorDefs *mtrDefs, bool redAlliance){
	pros::Task::delay(3000);
	mtrDefs->catapult_mtr->move(127);
	pros::Task::delay(500);
	mtrDefs->catapult_mtr->move(0);
	pros::Task::delay(500);

	
	//Bring the catapult down to loading position.
	pros::ADIDigitalIn bumper('E');
	mtrDefs->catapult_mtr->move(127);
	while(bumper.get_value()){
		pros::Task::delay(50);
	}
	mtrDefs->catapult_mtr->move(0);

	loadBallFromBack(mtrDefs, redAlliance);
	parkOnPlatform(mtrDefs, redAlliance);
}

void noAuton(){
}