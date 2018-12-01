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
	driveRobot(mtrDefs, -100, 950);
}

void alignAndShoot(MotorDefs *mtrDefs){
	pros::Task::delay(500);
	driveRobot(mtrDefs, 70, 225);
	pros::Task::delay(500);
	//turn to face flags
	turnDegrees(mtrDefs, 83, true /* turn left */);
	pros::Task::delay(400);
	mtrDefs->catapult_mtr->move_relative(415, 127);
	mtrDefs->intake_mtr->move(0);
	pros::Task::delay(3000);
}

void flipBottomFlagAndBackToTile(MotorDefs *mtrDefs){
	turnDegrees(mtrDefs, 15, true /* turn left */);
	driveRobot(mtrDefs, 100, 1000);
	pros::Task::delay(300);
	driveRobot(mtrDefs, -100, 875);
	pros::Task::delay(200);
	
	turnDegrees(mtrDefs, 83, false /* turn right */);
	pros::Task::delay(200);
	driveRobot(mtrDefs, -100, 550);
	pros::Task::delay(200);
}

void flipCap(MotorDefs *mtrDefs){
	mtrDefs->intake_mtr->move(-127);
	pros::Task::delay(500); 
	turnDegrees(mtrDefs, 105, true /* turn left */);
		
	driveRobot(mtrDefs, 70, 1250); //was 1300
	mtrDefs->intake_mtr->move(0);
	pros::Task::delay(1000);
	driveRobot(mtrDefs, -60, 1225);
	pros::Task::delay(1000);
}

void flipMidLowerFlag(MotorDefs *mtrDefs){
	driveRobot(mtrDefs, 127, 700);
	pros::Task::delay(100);
	turnDegrees(mtrDefs, 93, true /* turn left */);
	
	pros::Task::delay(100);
	driveRobot(mtrDefs, 100, 1500);
	pros::Task::delay(100);
	driveRobot(mtrDefs, -100, 500);
	pros::Task::delay(100);
}

void alignAgainstRedPlatform(MotorDefs *mtrDefs) {
	turnDegrees(mtrDefs, 60, false /* turn right */);
	pros::Task::delay(1000);
	driveRobot(mtrDefs, -60, 775);
	pros::Task::delay(1000);
}

void climbFirstPlatform(MotorDefs *mtrDefs) {
	// Climb up the platform.
	driveRobot(mtrDefs, -100, 1000);
	pros::Task::delay(1000);
}

void climbSecondPlatform(MotorDefs *mtrDefs) {
	// Climb up the platform.
	driveRobot(mtrDefs, -100, 1200);
	pros::Task::delay(1000);
}

void frontAuton(MotorDefs *mtrDefs){
	pickupAnotherBallAndComeBack(mtrDefs);
	alignAndShoot(mtrDefs);
	flipBottomFlagAndBackToTile(mtrDefs);
	flipMidLowerFlag(mtrDefs);
 	flipCap(mtrDefs);
	alignAgainstRedPlatform(mtrDefs);
	climbFirstPlatform(mtrDefs);
	//climbSecondPlatform(mtrDefs);
}