#include "okapi/api.hpp"
#include "autonselection.h"
#include "motordefs.hpp"
#include "okapi/api/chassis/controller/chassisController.hpp"

using namespace okapi;

const float TURN_SCALE_FACTOR = 4.1;
static MotorDefs mtrDefs;


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

okapi::Motor left_mtr_f(19, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); //default motor color
okapi::Motor right_mtr_f(14, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); //default motor color
okapi::Motor left_mtr_b(11, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); //default motor color
okapi::Motor right_mtr_b(2, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); //default motor color
okapi::Motor right_mtr_m(4, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); //default motor color
okapi::Motor left_mtr_m(1, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees); //default motor color

auto chassis = ChassisControllerFactory::create(
	{left_mtr_f, left_mtr_b},
	{right_mtr_f, right_mtr_b},
	//ADIEncoder('A', 'B', true), ADIEncoder('C', 'D'),
	IterativePosPIDController::Gains{0.001, 0.001, 0.09},
	IterativePosPIDController::Gains{0, 0, 0},
	IterativePosPIDController::Gains{0.0008, 0.001, 0.001},
	AbstractMotor::gearset::green,
	{4.5_in, 16_in}
);

void turnRight(int degrees){
	chassis.turnAngle(degrees * 2.2555556_deg); //2.255556 is the scale;
}
void turnLeft(int degrees){
	chassis.turnAngle(degrees * -2.2555556_deg); //2.255556 is the scale;
}
/*
void autonomous() {
	turnDegrees(90);
	pros::Task::delay(1000);
	turnDegrees(180);
	pros::lcd::set_text(3, "done turning");
}
*/
void autonomous() {
	chassis.turnAngle(90_deg);
	/*
	for (int i = 0; i < 10; ++i) {
    	chassis.turnAngle(45_deg);
	
		for (int i = 0; i < 100; ++i) {
			int32_t mtr1Vel = pros::c::motor_get_target_velocity(19);
			int32_t mtr2Vel = pros::c::motor_get_target_velocity(14);
			
			if (mtr1Vel != 0) {
				printf("mtr 1 vel: %ld\n", mtr1Vel);
			} else {
				printf("mtr 1 vel: %ld\n", 0);
			}

			if (mtr2Vel != 0) {
				printf("mtr 2 vel: %ld\n", mtr2Vel);
			} else {
				printf("mtr 1 vel: %ld\n", 0);
			}
			
			pros::Task::delay(10);
		}

	}*/
}
/*
void driveRobot(MotorDefs *mtrDefs, int power, int travelTime){
	left_mtr_f.move(power);
	left_mtr_b.move(power);
	right_mtr_f.move(power);
	right_mtr_b.move(power);
	right_mtr_m.move(power);
	left_mtr_m.move(power);
	pros::Task::delay(travelTime);
	left_mtr_f.move(-10);
	right_mtr_f.move(-10);
	left_mtr_b.move(-10);
	right_mtr_b.move(-10);
	right_mtr_m.move(-10);
	left_mtr_m.move(-10);
	pros::Task::delay(100);
	left_mtr_f.move(0);
	right_mtr_f.move(0);
	left_mtr_b.move(0);
	right_mtr_b.move(0);
	right_mtr_m.move(0);
	left_mtr_m.move(0);
}

void pickupAnotherBallAndComeBack(MotorDefs *mtrDefs){
	pros::Task::delay(200);
	driveRobot(mtrDefs, 80, 1200);
	intake_mtr.move(127);
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
		turnLeft(90);
	} else {
		turnRight(90);
	}
	pros::Task::delay(400);
	catapult_mtr.move_relative(415, 127);
	intake_mtr.move(0);
	pros::Task::delay(1000);
}

void flipBottomFlagAndBackToTile(MotorDefs *mtrDefs, bool redAlliance){
	if (redAlliance) {
		turnLeft(16); //test
	} else {
		turnRight(3); //test
	}
	driveRobot(mtrDefs, 100, 1000);
	pros::Task::delay(300);
	driveRobot(mtrDefs, -100, 825);
	pros::Task::delay(200);
	
	if (redAlliance) {
		turnLeft(90);
	} else {
		turnRight(90);
	}
	pros::Task::delay(200);
	driveRobot(mtrDefs, -100, 550);
	pros::Task::delay(200);
}

void flipCap(MotorDefs *mtrDefs, bool redAlliance){
	intake_mtr.move(-127);
	pros::Task::delay(500); 
	if(redAlliance){
		turnLeft(90);
	} else {
		turnRight(90); //test
	}
	
	driveRobot(mtrDefs, 70, 1250); //was 1300
	intake_mtr.move(0);
	pros::Task::delay(200);
	driveRobot(mtrDefs, -60, 700);	
}

void flipMidLowerFlag(MotorDefs *mtrDefs, bool redAlliance){
	driveRobot(mtrDefs, 127, 700);
	pros::Task::delay(100);
	if(redAlliance){
		turnLeft(90);
	} else {
		turnRight(90);
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
		turnRight(90);
		driveRobot(mtrDefs, -50, 300);
	} else {
		turnLeft(90);
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
		turnRight(52); //test
	} else {
		turnLeft(60); //test
	}
	pros::Task::delay(200);

	// Pick up ball from under the cap and come straight back to hit the fence
	// to ensure straight alignment.
	pickupAnotherBallAndComeBack(mtrDefs);
	pros::Task::delay(200);
	driveRobot(mtrDefs, -70, 200);
	pros::Task::delay(200);

	// Stop intake
	intake_mtr.move(0);
}

void parkOnPlatform(MotorDefs *mtrDefs, bool redAlliance){
	// Move forward a bit so that we can clear the fence in preparation for turning.
	driveRobot(mtrDefs, 70, 200);
	pros::Task::delay(200);

	// Turn so that we go in front of the platform.
	if (redAlliance) {
		turnLeft(90);
	} else {
		turnRight(90);
	}
	pros::Task::delay(200);

	//Move forward so that the robot is within platform's range
	driveRobot(mtrDefs, 70, 800);
	pros::Task::delay(200);

	// Turn towards the fence so that back of the robot is facing the platform.
	if (redAlliance) {
		turnLeft(90);
	} else {
		turnRight(90);
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
	catapult_mtr.move(127);
	pros::Task::delay(500);
	catapult_mtr.move(0);
	pros::Task::delay(500);

	
	//Bring the catapult down to loading position.
	pros::ADIDigitalIn bumper('E');
	catapult_mtr.move(127);
	while(bumper.get_value()){
		pros::Task::delay(50);
	}
	catapult_mtr.move(0);

	loadBallFromBack(mtrDefs, redAlliance);
	parkOnPlatform(mtrDefs, redAlliance);
}

void noAuton(){
}

*/