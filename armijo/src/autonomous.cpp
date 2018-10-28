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
void frontAuton(bool redAlliance);
void backAuton();
void noAuton();

void autonomous() {
	switch (autonSelected) {
		case 0:
			pros::lcd::set_text(2, "front red 3 flag running!");
			frontAuton(true /* red alliance */);
			break;
		case 1:
			pros::lcd::set_text(2, "front blue 3 flag running!");
			frontAuton(false /* blue alliance */);
			break;
		case 2:
			pros::lcd::set_text(2, "back auton");
			backAuton();
			break;
		default:
			pros::lcd::set_text(2, "no auton");
			noAuton();
			break;
	}
}

void drive(MotorDefs *mtr_defs, int power, int travelTime){
	mtr_defs->left_mtr_f->move(power);
	mtr_defs->left_mtr_b->move(power);
	mtr_defs->right_mtr_f->move(power);
	mtr_defs->right_mtr_b->move(power);
	pros::Task::delay(travelTime);
	mtr_defs->left_mtr_f->move(0);
	mtr_defs->right_mtr_f->move(0);
	mtr_defs->left_mtr_b->move(0);
	mtr_defs->right_mtr_b->move(0);
}

void frontAuton(bool redAlliance){
	MotorDefs *mtr_defs = MotorDefs::getMotorDefs();
	if (mtr_defs == NULL) {
		pros::lcd::set_text(2, "MotorDefs is NULL!");
	} else if (mtr_defs->left_mtr_f == NULL) {
		pros::lcd::set_text(2, "left motor front is NULL!");
	} else {
		auto driveTrain = ChassisControllerFactory::create(
							*(mtr_defs->left_mtr_f), 
							*(mtr_defs->right_mtr_f),
							*(mtr_defs->right_mtr_b), 
							*(mtr_defs->left_mtr_b), 
							AbstractMotor::gearset::green,
							{4_in, 15.5_in});
		pros::Task::delay(200);
		drive(mtr_defs, 127, 1000);
		mtr_defs->intake_mtr->move(127);
		pros::Task::delay(300);
		// move back with ball and preload ball towards fence
		drive(mtr_defs, -127, 1100);
		pros::Task::delay(500);
		if (redAlliance) {
			driveTrain.moveDistance(14_in);
		} else {
			driveTrain.moveDistance(4_in);
		}	
		pros::Task::delay(100);

		//turn to face the flags
		if (redAlliance){
			driveTrain.turnAngle(-1*90_deg);
			pros::Task::delay(200);
		} else {
			driveTrain.turnAngle(84_deg);
		}
		pros::Task::delay(200);
		pros::Task::delay(400);
		mtr_defs->catapult_mtr->move_relative(415, 127);
		pros::Task::delay(500);
		mtr_defs->intake_mtr->move(0);
		if (redAlliance) {
			driveTrain.turnAngle(-1*15_deg);
		} else {
			driveTrain.turnAngle(7_deg);
		}
		
		driveTrain.moveDistance(45_in);
		pros::Task::delay(300);
		driveTrain.moveDistance(-1*20_in);
	}
}


void backAuton(){
	MotorDefs *mtr_defs = MotorDefs::getMotorDefs();
	Motor catapult_mtr(5, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
	pros::Task::delay(12000);
	mtr_defs->catapult_mtr->move(127);
	pros::Task::delay(500);
	mtr_defs->catapult_mtr->move(0);
}

void noAuton(){
}