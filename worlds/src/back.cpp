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

void Back::shootMiddleFlag(){
	// Hold power on flipper so that it doesn't come down
	mtrDefs->flipper_mtr->move(10);

	// Lets wait for 5 seconds
	pros::Task::delay(1000);

	// Shoot middle pole top flag
	commonAutons->shootCatapult();
}



void Back::pickupBallAndShootOpponentFlag(){
	// Align against fence and pick up ball from underneath the cap
	pros::Task::delay(100);
	pros::Task cataLoadTask(Commons::catapultLoad, mtrDefs);
	pros::Task flipperTask(Commons::flipperCapPos, mtrDefs);
	robotDriver->driveRobot(-75, 30);
	if(redAlliance){
		robotDriver->turnRobot(45, false);
	} else {
		robotDriver->turnRobot(55, true);
	}
	//hit the fence
	robotDriver->driveWithCoast(1000, -30);
	//turn on intake and go forward
	mtrDefs->intake_mtr->move(127);
	robotDriver->smoothDrive(800, 127, 1);
	robotDriver->driveWithCoast(500, 80);
	//come back slightly to be centered with the platform
	// the distance is different as the platform is closer to the blue side by one tile tick
	pros::Task flipperUpTask(Commons::flipperUp, mtrDefs);
	if(redAlliance){
		robotDriver->driveRobot(-30, 50);
	} else{
		robotDriver->driveRobot(-55, 50);
	}
	
	// Align and shoot opponent's flag
	pros::Task::delay(100);
	//turn to face platform
	if(redAlliance){
		robotDriver->turnRobot(90, true);
	} else {
		robotDriver->turnRobot(90, false);
	}
	//align with platform
	robotDriver->driveWithCoast(700, 50);
	pros::Task::delay(200);
	// Drive back and turn to face opponent's flag
	robotDriver->driveRobot(-400, 50);
	if(redAlliance){
		robotDriver->turnRobot(36, false);
	} else {
		robotDriver->turnRobot(30, true);
	}
	// Shoot opponent's flag
	pros::Task::delay(500);
	commonAutons->shootCatapult();
	pros::Task::delay(200);
}

void Back::park(){
	// Turn to face the platform
	if(redAlliance){
		robotDriver->turnRobot(36, true);
	} else {
		robotDriver->turnRobot(30, false);
	}
	// Let the robot stabilize before shooting
	pros::Task::delay(1000);
	// Drive and park on platform
	robotDriver->driveWithCoast(1550, 100);
	mtrDefs->right_mtr_f->move_relative(0, 200);
	mtrDefs->middle_mtr->move_relative(0, 200);
	mtrDefs->right_mtr_b->move_relative(0, 200);
	mtrDefs->left_mtr_f->move_relative(0, 200);
	mtrDefs->left_mtr_b->move_relative(0, 200);
}

void Back::runAuton(){
	shootMiddleFlag();
	pickupBallAndShootOpponentFlag();
	park();
}
