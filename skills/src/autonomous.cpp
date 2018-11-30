#include "okapi/api.hpp"
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
void frontAuton(MotorDefs *mtrDefs);

void autonomous() {
	MotorDefs mtrDefs;
	frontAuton(&mtrDefs);
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
	driveRobot(mtrDefs, -100, 1050);
}

void alignAndShoot(MotorDefs *mtrDefs){
	pros::Task::delay(500);
	driveRobot(mtrDefs, 70, 225);
	pros::Task::delay(500);
	//turn to face flags
	turnDegrees(mtrDefs, 85, true /* turn left */); //was 83
	pros::Task::delay(400);
	mtrDefs->catapult_mtr->move_relative(415, 127);
	mtrDefs->intake_mtr->move(0);
	pros::Task::delay(500);
}

void flipBottomFlagAndBackToTile(MotorDefs *mtrDefs){
	turnDegrees(mtrDefs, 18, true /* turn left */);
	driveRobot(mtrDefs, 100, 1000);
	pros::Task::delay(300);
	driveRobot(mtrDefs, -100, 750);
	pros::Task::delay(200);
	
	turnDegrees(mtrDefs, 83, false /* turn right */);
	pros::Task::delay(200);
	driveRobot(mtrDefs, -100, 750);
	pros::Task::delay(200);
}

void flipCap(MotorDefs *mtrDefs){
	mtrDefs->intake_mtr->move(-127);
	pros::Task::delay(500); 
	turnDegrees(mtrDefs, 105, true /* turn left */);
		
	driveRobot(mtrDefs, 70, 1250); //was 1300
	mtrDefs->intake_mtr->move(0);
	pros::Task::delay(200);
	driveRobot(mtrDefs, -60, 700);	
}

void flipMidLowerFlag(MotorDefs *mtrDefs){
	driveRobot(mtrDefs, 127, 600);
	pros::Task::delay(100);
	turnDegrees(mtrDefs, 87, true /* turn left */);
	
	pros::Task::delay(100);
	driveRobot(mtrDefs, 100, 1500);
	pros::Task::delay(100);
	driveRobot(mtrDefs, -100, 450);
	pros::Task::delay(100);
}

void parkOnPlatform(MotorDefs *mtrDefs){
	// Move forward a bit so that we can clear the fence in preparation for turning.
	driveRobot(mtrDefs, 70, 200);
	pros::Task::delay(200);

	// Turn so that we go in front of the platform.
	turnDegrees(mtrDefs, 90, true /* right left */ );
	pros::Task::delay(200);

	//Move forward so that the robot is within platform's range
	driveRobot(mtrDefs, 70, 800);
	pros::Task::delay(200);

	// Turn towards the fence so that back of the robot is facing the platform.
	turnDegrees(mtrDefs, 95, true /*turn left */);
	
	pros::Task::delay(200);

	// Move baack until the back wheels are aligned and touching the foot of the platform.
	driveRobot(mtrDefs, -50, 500);
	pros::Task::delay(500);

	// Climb up the platform.
	driveRobot(mtrDefs, -100, 1200);
	pros::Task::delay(200);
}


void frontAuton(MotorDefs *mtrDefs){
	pickupAnotherBallAndComeBack(mtrDefs);
	alignAndShoot(mtrDefs);
	flipBottomFlagAndBackToTile(mtrDefs);
	flipMidLowerFlag(mtrDefs);
 	flipCap(mtrDefs);
}