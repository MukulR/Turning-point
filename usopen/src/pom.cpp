#include "pom.hpp"
#include "commons.hpp"
#include "motordefs.hpp"
#include "robot_driver.hpp"

POM::POM(MotorDefs *md, RobotDriver *rd, bool ra, Commons *ca){
    mtrDefs = md;
    robotDriver = rd;
    redAlliance = ra;
    commonAutons = ca;
}

POM::~POM() {}

void POM::getPlatformBallAndAlignAgainstFence(){
	// Drive to the ball on platform
	robotDriver->smoothDrive(350, 80, 1);
	
	// Pickup the ball
	mtrDefs->intake_mtr->move(127);
	mtrDefs->flipper_mtr->move(-40);
	pros::Task::delay(350);
	robotDriver->driveRobot(-50, 50);
	mtrDefs->flipper_mtr->move(0);
	pros::Task::delay(100);

	// Drive back to starting position
	robotDriver->smoothDrive(350, 80, -1);

	// Turn left if red alliance or right if blue alliance
	if(redAlliance){
		robotDriver->turnRobot(33, true);
	} else {
		robotDriver->turnRobot(18, false);
	}

	// Drive back so that the back of the robot aligns against the fence and bring up flipper
	robotDriver->driveWithCoast(800, -50);
	mtrDefs->flipper_mtr->move(110);
	pros::Task::delay(250);
	mtrDefs->flipper_mtr->move(5);
}

void POM::runAuton(){
	getPlatformBallAndAlignAgainstFence();
	commonAutons->alignAndShootOurFlags();
	//toggleLowFlag(mtrDefs, redAlliance);
	commonAutons->pickupBallsFromCapFlipAndShoot();
}
