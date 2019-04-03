#include "autonselection.hpp"
#include "commons.hpp"
#include "motordefs.hpp"
#include "pom.hpp"
#include "robot_driver.hpp"
#include "uom.hpp"

/*
pros::ADIDigitalIn bumper_auton('C');
pros::ADIPotentiometer potentiometer('H');
*/

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
//void backAuton(MotorDefs *mtrDefs, RobotDriver *robotDriver, bool redAlliance);
void noAuton();

void autonomous() {
	MotorDefs mtrDefs;
	mtrDefs.initPIDVals();
	RobotDriver robotDriver(&mtrDefs);
	Commons commonAutons(&mtrDefs, &robotDriver, redAlliance);
	
	switch (autonSelected) {
		case 0:
			{
				POM pomAuton(&mtrDefs, &robotDriver, redAlliance, &commonAutons);
				pomAuton.runAuton();
			}
			break;
		case 1:
			{
				UOM uomAuton(&mtrDefs, &robotDriver, redAlliance, &commonAutons);
				uomAuton.runAuton();
			}
			break;
		case 2:
			//backAuton();
			break;
		default:
			noAuton();
			break;
	}
}

/*
void backAuton(MotorDefs *mtrDefs, RobotDriver *robotDriver, bool redAlliance){
	// Hold power on flipper so that it doesn't come down
	mtrDefs->flipper_mtr->move(5);

	// Lets wait for 5 seconds
	pros::Task::delay(5000);

	// Shoot middle pole top flag
	shootCatapult(mtrDefs);
	// Align against fence and pick up ball from underneath the cap
	pros::Task::delay(100);
	pros::Task cataLoadTask(catapultLoad, mtrDefs);
	robotDriver->driveRobot(-75, 30);
	if(redAlliance){
		robotDriver->turnRobot(45, false);
	} else {
		robotDriver->turnRobot(45, true);
	}
	//hit the fence
	robotDriver->driveWithCoast(1000, -30);
	mtrDefs->intake_mtr->move(127);
	robotDriver->smoothDrive(1000, 127, 1);
	// Align and shoot opponent's flag
	pros::Task::delay(100);
	//drive back so that the turn can be in the middle of the platform
	//smoothDrive(10, 80, -1);
	//turn to face platform
	if(redAlliance){
		robotDriver->turnRobot(90, true);
	} else {
		robotDriver->turnRobot(90, false);
	}
	//align with platform
	robotDriver->driveWithCoast(700, 30);
	pros::Task::delay(200);
	// Drive back and turn to face opponent's flag
	robotDriver->driveRobot(-150, 50);
	if(redAlliance){
		robotDriver->turnRobot(30, false);
	} else {
		robotDriver->turnRobot(30, true);
	}
	// Shoot opponent's flag
	pros::Task::delay(500);
	shootCatapult(mtrDefs);
	pros::Task::delay(200);
	// Turn to face the platform
	if(redAlliance){
		robotDriver->turnRobot(30, true);
	} else {
		robotDriver->turnRobot(30, false);
	}
	// Let the robot stabilize before shooting
	pros::Task::delay(1000);
	// Drive and park on platform
	robotDriver->driveWithCoast(1100, 100);
}
*/

void noAuton(){}