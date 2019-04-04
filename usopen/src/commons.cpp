
#include "commons.hpp"
#include "motordefs.hpp"
#include "robot_driver.hpp"

pros::ADIDigitalIn bumper_auton('C');
pros::ADIPotentiometer potentiometer('H');

Commons::Commons(MotorDefs *md, RobotDriver *rd, bool ra) {
    mtrDefs = md;
    robotDriver = rd;
    redAlliance = ra;
}

Commons::~Commons() {}

void Commons::catapultLoad(void* param){
	MotorDefs *mtrDefs = (MotorDefs*)param;
	mtrDefs->catapult_mtr->move(127);
	pros::Task::delay(100);
	while(bumper_auton.get_value()){
		pros::Task::delay(50);
	}
	mtrDefs->catapult_mtr->move(0);
}

void Commons::shootCatapult() {
	mtrDefs->catapult_mtr->move_relative(500, 127);
	pros::Task::delay(50);
	while(mtrDefs->catapult_mtr->get_actual_velocity() != 0 || mtrDefs->catapult_mtr->is_stopped() == 0){
		pros::Task::delay(10);
	}
}

void Commons::flipperMove(int potValue, int power, int direction){
	mtrDefs->flipper_mtr->move(direction * power);
	if(direction == 1){ // Bring flipper up
		while(potentiometer.get_value() > potValue){
			pros::Task::delay(5);
		}
	} else { // Bring flipper down
		while(potentiometer.get_value() < potValue){
			pros::Task::delay(5);
		}
	}
	mtrDefs->flipper_mtr->move_relative(0, 200);
}

void Commons::pickupBallsFromCapFlipAndShoot() {
    //bring catapult down to loading position asynchronously using a task.
	pros::Task cataLoadTask(Commons::catapultLoad, mtrDefs);

	//come back so that we compensate for the distance we would have traveled to shoot the flags
	robotDriver->driveRobot(-150, 50);

	//align to the cap 
	if(redAlliance){
		robotDriver->turnRobot(40, false);
	} else {
		robotDriver->turnRobot(45, true);
	}
	
	// Move forward so that when we bring the flipper down it is well over the peg of the
	// cap.
	robotDriver->driveRobot(420, 80);

	// Bring the flipper down
	flipperMove(2500, 80, -1);

	//block until catapultloadtask completes
	
	// Start the intake
	mtrDefs->intake_mtr->move(127);

	// Now drive back a bit so that cap bends towards the robot so that balls
	// fall into intake
	if(redAlliance){
		robotDriver->driveRobot(-200, 100);
	} else{
		robotDriver->driveRobot(-200, 100);
	}

	// Bring the flipper up so that we let go of the cap
	flipperMove(315, 127, 1);
	pros::Task::delay(200);

	// Bring flipper down so that when we move forward cap doesn't get stuck in intake
	robotDriver->driveRobot(-125, 80);
	flipperMove(3320, 127, -1);
	
	// Move forward a bit so that we are within range of top two middle flags

	robotDriver->driveRobot(475, 80);

	// Bring flipper back up with the hope that we flip the cap
	flipperMove(315, 90, 1);

	// Turn to make sure we can hit the flags
	if(redAlliance){
		robotDriver->turnRobot(10, false);
	} else{
		robotDriver->turnRobot(9, false);	
	}//turn right for blue as catapult is on the left side already 


	// Add a slight delay so that robot loads the balls properly
	pros::Task::delay(1500);

	// Shoot top two middle flags
	shootCatapult();
}

void Commons::alignAndShootOurFlags() {
    if(redAlliance){
		robotDriver->driveRobot(225, 50);
	} else{
		robotDriver->driveRobot(150, 50);
	}
	pros::Task::delay(100);
	if(redAlliance){
		robotDriver->turnRobot(88, true);
	} else {
		robotDriver->turnRobot(90, false);
	}
	pros::Task::delay(50);
	robotDriver->driveRobot(250, 50);
	pros::Task::delay(200);
	shootCatapult();
	mtrDefs->intake_mtr->move(0);
}

void Commons::toggleLowFlag() {
    if(redAlliance){
		robotDriver->turnRobot(12, true);
	} else {
		robotDriver->turnRobot(5, false);
	}
	robotDriver->smoothDrive(850, 127, 1);
	pros::Task::delay(100);
	robotDriver->smoothDrive(850, 127, -1);
}