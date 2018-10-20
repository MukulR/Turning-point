//#include "main.h"
#include "okapi/api.hpp"
#include "autonselection.h"

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

void frontAuton(bool redAlliance){
	Motor left_mtr_f(1, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
	Motor left_mtr_b(3, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
	Motor right_mtr_f(2, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
	Motor right_mtr_b(4, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
	Motor intake_mtr(6, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
	Motor catapult_mtr(5, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
	

	auto drive = ChassisControllerFactory::create(left_mtr_f, right_mtr_f, right_mtr_b, left_mtr_b, 
												AbstractMotor::gearset::green, {4_in, 15.5_in});
	left_mtr_f.setVoltageLimit(9000);
	left_mtr_b.setVoltageLimit(9000);
	right_mtr_f.setVoltageLimit(9000);
	right_mtr_b.setVoltageLimit(9000);
	//intake_mtr.move(127);
	pros::Task::delay(200);
	drive.moveDistance(40_in);
	intake_mtr.move(127);
	drive.moveDistance(2_in);
	pros::Task::delay(300);

	// turning to not hit the pole
	// move back with ball and preload ball towards fence
	left_mtr_f.move(-75);
	left_mtr_b.move(-75);
	right_mtr_f.move(-75);
	right_mtr_b.move(-75);
	pros::Task::delay(1500);
	left_mtr_f.move(0);
	right_mtr_f.move(0);
	left_mtr_b.move(0);
	right_mtr_b.move(0);

	pros::Task::delay(500);
	if (redAlliance) {
		drive.moveDistance(14_in);
	} else {
		drive.moveDistance(4_in);
	}	
	pros::Task::delay(100);

	//turn to face the flags
	if (redAlliance){
		drive.turnAngle(-1*90_deg);
	} else {
		drive.turnAngle(78_deg);
	}
	pros::Task::delay(200);
	drive.moveDistance(4_in);
	pros::Task::delay(200);
	catapult_mtr.move_relative(415, 127);
	pros::Task::delay(500);
	intake_mtr.move(0);
	if (redAlliance) {
		drive.turnAngle(-1*15_deg);
	} else {
		drive.turnAngle(7_deg);
	}
	
	drive.moveDistance(45_in);
	pros::Task::delay(300);
	drive.moveDistance(-1*20_in);
}


void backAuton(){
	Motor catapult_mtr(5, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
	Motor brake_mtr(7, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
	brake_mtr.move(-127);
	pros::Task::delay(300);
	brake_mtr.move(-10);
	pros::Task::delay(12000);
	catapult_mtr.move(127);
	pros::Task::delay(400);
	catapult_mtr.move(0);
}

void noAuton(){
}