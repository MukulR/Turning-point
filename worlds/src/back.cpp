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

/** DEPRECATED **/
void Back::shootMiddleFlag(){
	// Hold power on flipper so that it doesn't come down
	mtrDefs->flipper_mtr->move(10);

	robotDriver->driveRobot(80, 50);
	// Shoot middle pole top flag
	pros::Task::delay(50);
	commonAutons->shootCatapult();
	robotDriver->driveRobot(-80, 50);
}

/** DEPRECATED **/
void Back::pickupBallAndShootOpponentFlag(){
	// Align against fence and pick up ball from underneath the cap
	pros::Task cataLoadTask(Commons::catapultLoad, mtrDefs);
	//pros::Task flipperTask(Commons::flipperCapPos, mtrDefs);
	robotDriver->driveRobot(-120, 30);
	if(redAlliance){
		robotDriver->turnRobot(52, false);
	} else {
		robotDriver->turnRobot(58, true);
	}
	//hit the fence
	robotDriver->driveWithCoast(1000, -30);
	//turn on intake and go forward
	mtrDefs->intake_mtr->move(127);
	robotDriver->smoothDrive(800, 127, 1);
	robotDriver->driveWithCoast(600, 30);
	//come back slightly to be centered with the platform
	// the distance is different as the platform is closer to the blue side by one tile tick
	
	if(redAlliance){
		robotDriver->driveRobot(-75, 50);
	} else{
		robotDriver->driveRobot(-50, 50);
	}

	// Align and shoot opponent's flag
	pros::Task::delay(100);
	//turn to face platform
	if(redAlliance){
		robotDriver->turnRobot(90, true);
	} else {
		robotDriver->turnRobot(90, false);
	}
	//pros::Task flipperUpTask(Commons::flipperUp, mtrDefs);
	//align with platform
	robotDriver->driveWithCoast(700, 50);
	pros::Task::delay(200);
	// Drive back and turn to face opponent's flag
	robotDriver->driveRobot(-400, 50);
	if(redAlliance){
		robotDriver->turnRobot(20, false);
	} else {
		robotDriver->turnRobot(20, true);
	}
	// Shoot opponent's flag
	pros::Task::delay(500);
	commonAutons->shootCatapult();
	pros::Task::delay(200);
}

/** DEPRECATED **/
void Back::park(){
	// Turn to face the platform
	if(redAlliance){
		robotDriver->turnRobot(30, true);
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
	/*
	shootMiddleFlag();
	pickupBallAndShootOpponentFlag();
	park();
	*/

	// Wait for opponent to toggle their flags, then shoot
	pros::Task::delay(8000);
	mtrDefs->catapult_mtr->move_relative(500, 127);
	pros::Task::delay(300);

	//turn so that we are parallel to the fence and align with it.
	if(redAlliance){
		robotDriver->turnRobot(40, false);
	} else {
		robotDriver->turnRobot(50, true);
	}
	pros::Task::delay(50);
	robotDriver->driveWithCoast(1000, -30);
	pros::Task::delay(100);

	//taskify the flipper and catapult reload, head towards cap and pick up ball.
	pros::Task flipperTask(Commons::flipperCapPos, mtrDefs);
	pros::Task cataLoadTask(Commons::catapultLoad, mtrDefs);
	mtrDefs->intake_mtr->move(127);
	robotDriver->smoothDrive(800, 127, 1);
	robotDriver->driveWithCoast(1000, 30);
	pros::Task::delay(50);

	//drive back so that we are aligned to the platform when we turn. 
	//(Distances different because of platform alignment on the field)
	if(redAlliance){
		robotDriver->driveRobot(-75, 50);
	} else{
		robotDriver->driveRobot(-120, 50);
	}

	pros::Task::delay(100);

	//bring the flipper up, turn to face the platform, and park.
	pros::Task flipperUpTask(Commons::flipperUp, mtrDefs);
	if(redAlliance){
		robotDriver->turnRobot(90, true);
	} else {
		robotDriver->turnRobot(90, false);
	}
	robotDriver->driveWithCoast(1200, 100);
	mtrDefs->right_mtr_f->move_relative(0, 200);
	mtrDefs->middle_mtr->move_relative(0, 200);
	mtrDefs->right_mtr_b->move_relative(0, 200);
	mtrDefs->left_mtr_f->move_relative(0, 200);
	mtrDefs->left_mtr_b->move_relative(0, 200);
}
