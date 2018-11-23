//#include "main.h"
#include "okapi/api.hpp"
#include "autonselection.h"
#include "motordefs.hpp"

using namespace okapi;


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

void setVoltageLimit(MotorDefs *mtrDefs, int voltageLimit) {
	mtrDefs->left_mtr_b->setVoltageLimit(voltageLimit);
	mtrDefs->left_mtr_f->setVoltageLimit(voltageLimit);
	mtrDefs->right_mtr_f->setVoltageLimit(voltageLimit);
	mtrDefs->right_mtr_b->setVoltageLimit(voltageLimit);
	mtrDefs->right_mtr_m->setVoltageLimit(voltageLimit);
	mtrDefs->left_mtr_m->setVoltageLimit(voltageLimit);
}

void pickupAnotherBallAndComeBack(MotorDefs *mtrDefs){
	pros::Task::delay(200);
	driveRobot(mtrDefs, 80, 1200);
	mtrDefs->intake_mtr->move(127);
	pros::Task::delay(300);
	// move back with ball and preload ball towards fence
	driveRobot(mtrDefs, -80, 1300);
}

void alignAndShoot(ChassisControllerIntegrated *drive, MotorDefs *mtrDefs, bool redAlliance){
	pros::Task::delay(500);
	if(redAlliance == true){
		driveRobot(mtrDefs, 70, 325);
	} else {
		driveRobot(mtrDefs, 70, 100);
	}

	pros::Task::delay(800);
	//turn to face the flags
	setVoltageLimit(mtrDefs, 8000);
	if (redAlliance){
		drive->turnAngle(-1*85_deg);
		pros::Task::delay(200);
	} else {
		drive->turnAngle(81_deg);
	}
	setVoltageLimit(mtrDefs, 12000);
	pros::Task::delay(400);
	mtrDefs->catapult_mtr->move_relative(415, 127);
	pros::Task::delay(500);
	mtrDefs->intake_mtr->move(0);
}

void flipBottomFlagAndFaceCap(ChassisControllerIntegrated *drive, MotorDefs *mtrDefs, bool redAlliance){
	if (redAlliance) {
		drive->turnAngle(-1*12_deg);
	} 
	driveRobot(mtrDefs, 100, 1500);//drive.moveDistance(45_in);
	pros::Task::delay(300);
	drive->moveDistance(-1*25_in);
	setVoltageLimit(mtrDefs, 8000); 
	pros::Task::delay(200);
	if (redAlliance) {
		drive->turnAngle(90_deg);
	} else {
		drive->turnAngle(-82_deg);
	}
}

void flipCap(MotorDefs *mtrDefs){
	setVoltageLimit(mtrDefs, 12000);
	mtrDefs->intake_mtr->move(-127); 
	driveRobot(mtrDefs, 60, 1200);
	pros::Task::delay(200);
	mtrDefs->intake_mtr->move(0);
}

void flipMidLowerFlag(ChassisControllerIntegrated *drive, MotorDefs *mtrDefs, bool redAlliance){
	if(redAlliance){
		drive->turnAngle(-55_deg);
	} else {
		drive->turnAngle(55_deg);
	}
	pros::Task::delay(200);
	driveRobot(mtrDefs, 90, 800);
	pros::Task::delay(200);
	if(redAlliance){
		drive->turnAngle(-45_deg);
	} else {
		drive->turnAngle(45_deg);
	}
}

void parkOnPlatform(ChassisControllerIntegrated *drive, MotorDefs *mtrDefs, bool redAlliance){

}

void frontAuton(MotorDefs *mtrDefs, bool redAlliance, bool park){
	auto drive = ChassisControllerFactory::create({*mtrDefs->left_mtr_f, *mtrDefs->left_mtr_b, 
													*mtrDefs->left_mtr_m}, {*mtrDefs->right_mtr_f, 
													*mtrDefs->right_mtr_m, *mtrDefs->right_mtr_b},
													AbstractMotor::gearset::green, {4_in, 15.5_in});
	
	pickupAnotherBallAndComeBack(mtrDefs);
	alignAndShoot(&drive, mtrDefs, redAlliance);
	flipBottomFlagAndFaceCap(&drive, mtrDefs, redAlliance);
	flipCap(mtrDefs);
	if (park) {
		parkOnPlatform(&drive, mtrDefs, redAlliance);
	} else {
		flipMidLowerFlag(&drive, mtrDefs, redAlliance);
	}
	

	/*
	pros::Task::delay(200);
	driveRobot(mtrDefs, 80, 1200);
	mtrDefs->intake_mtr->move(127);
	pros::Task::delay(300);
	// move back with ball and preload ball towards fence
	driveRobot(mtrDefs, -80, 1300);
	
	pros::Task::delay(500);
	if(redAlliance == true){
		driveRobot(mtrDefs, 70, 325);
	} else {
		driveRobot(mtrDefs, 70, 100);
	}

	pros::Task::delay(800);
	//turn to face the flags
	setVoltageLimit(mtrDefs, 8000);
	if (redAlliance){
		drive.turnAngle(-1*85_deg);
		pros::Task::delay(200);
	} else {
		drive.turnAngle(81_deg);
	}
	setVoltageLimit(mtrDefs, 12000);
	pros::Task::delay(400);
	mtrDefs->catapult_mtr->move_relative(415, 127);
	pros::Task::delay(500);
	mtrDefs->intake_mtr->move(0);
	

	if (redAlliance) {
		drive.turnAngle(-1*12_deg);
	} else {
		//drive.turnAngle(3_deg);
	}
	
	driveRobot(mtrDefs, 100, 1500);//drive.moveDistance(45_in);
	pros::Task::delay(300);
	drive.moveDistance(-1*25_in);
	setVoltageLimit(mtrDefs, 8000); 
	pros::Task::delay(200);
	if (redAlliance) {
		drive.turnAngle(90_deg);
	} else {
		drive.turnAngle(-82_deg);
	}
	
	setVoltageLimit(mtrDefs, 12000);
	mtrDefs->intake_mtr->move(-127); 
	driveRobot(mtrDefs, 60, 1200);
	pros::Task::delay(200);
	mtrDefs->intake_mtr->move(0);
	
	if(redAlliance){
		drive.turnAngle(-55_deg);
	} else {
		drive.turnAngle(55_deg);
	}
	pros::Task::delay(200);
	driveRobot(mtrDefs, 90, 800);
	pros::Task::delay(200);
	if(redAlliance){
		drive.turnAngle(-45_deg);
	} else {
		drive.turnAngle(45_deg);
	}
	*/
}


void backAuton(MotorDefs *mtrDefs, bool redAlliance, bool park){
	pros::Task::delay(12000);
	mtrDefs->catapult_mtr->move(127);
	pros::Task::delay(500);
	mtrDefs->catapult_mtr->move(0);
}

void noAuton(){
}