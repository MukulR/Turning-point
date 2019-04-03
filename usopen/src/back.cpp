#include "back.hpp"
#include "commons.hpp"
#include "motordefs.hpp"
#include "robot_driver.hpp"

Back::Back(MotorDefs *md, RobotDriver *rd, bool ra, Commons *ca){
    mtrDefs = md;
    robotDriver = rd;
    redAlliance = ra;
    commonAutons = ca;
}

Back::~Back() {}


void Back::runAuton(){
	// Hold power on flipper so that it doesn't come down
	mtrDefs->flipper_mtr->move(5);

	// Lets wait for 5 seconds
	pros::Task::delay(5000);

	// Shoot middle pole top flag
	commonAutons->shootCatapult();
	// Align against fence and pick up ball from underneath the cap
	pros::Task::delay(100);
	pros::Task cataLoadTask(Commons::catapultLoad, mtrDefs);
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
	commonAutons->shootCatapult();
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
	robotDriver->driveWithCoast(1000, 100);
}
