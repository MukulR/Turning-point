#include "autonselection.hpp"
#include "motordefs.hpp"
#include "robot_driver.hpp"

pros::ADIDigitalIn bumper_auton('C');
pros::ADIPotentiometer potentiometer('H');

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

void getPlatformBallAndAlignAgainstFence(MotorDefs *mtrDefs, RobotDriver *robotDriver, bool redAlliance);
void pickupBallsFromCapFlipAndShoot(MotorDefs *mtrDefs, RobotDriver *robotDriver, bool redAlliance);
void alignAndShootOurFlags(MotorDefs *mtrDefs, RobotDriver *robotDriver, bool redAlliance);
void toggleLowFlag(MotorDefs *mtrDefs, RobotDriver *robotDriver, bool redAlliance);

void flipperLowFlag(MotorDefs *mtrDefs);

void pom(MotorDefs *mtrDefs, RobotDriver *robotDriver, bool redAlliance);
void backAuton(MotorDefs *mtrDefs, RobotDriver *robotDriver, bool redAlliance);
void noAuton();

void catapultLoad(void* param){
	MotorDefs *mtrDefs = (MotorDefs*)param;
	mtrDefs->catapult_mtr->move(127);
	pros::Task::delay(100);
	while(bumper_auton.get_value()){
		pros::Task::delay(50);
	}
	mtrDefs->catapult_mtr->move(0);
}


void flipperMove(MotorDefs* mtrDefs, int potValue, int power, int direction){
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

void autonomous() {
	MotorDefs mtrDefs;
	mtrDefs.initPIDVals();
	RobotDriver robotDriver(&mtrDefs);
	
	switch (autonSelected) {
		case 0:
			pom(&mtrDefs, &robotDriver, redAlliance);
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			backAuton(&mtrDefs, &robotDriver, redAlliance);
			break;
		case 5:
			break;
		case 6:
			break;
		default:
			noAuton();
			break;
	}
}

void shootCatapult(MotorDefs *mtrDefs){
	mtrDefs->catapult_mtr->move_relative(500, 127);
	pros::Task::delay(50);
	while(mtrDefs->catapult_mtr->get_actual_velocity() != 0 || mtrDefs->catapult_mtr->is_stopped() == 0){
		pros::Task::delay(10);
	}
}

void getPlatformBallAndAlignAgainstFence(MotorDefs *mtrDefs, RobotDriver *robotDriver, bool redAlliance){
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

void pom(MotorDefs *mtrDefs, RobotDriver *robotDriver, bool redAlliance){
	getPlatformBallAndAlignAgainstFence(mtrDefs, robotDriver, redAlliance);
	alignAndShootOurFlags(mtrDefs, robotDriver, redAlliance);
	//toggleLowFlag(mtrDefs, redAlliance);
	pickupBallsFromCapFlipAndShoot(mtrDefs, robotDriver, redAlliance);
}

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

void flipperCapPos(MotorDefs *mtrDefs){
	mtrDefs->flipper_mtr->move_relative(-400, 200);
}

void pickupBallsFromCapFlipAndShoot(MotorDefs *mtrDefs, RobotDriver *robotDriver, bool redAlliance){
	//bring catapult down to loading position asynchronously using a task.
	pros::Task cataLoadTask(catapultLoad, mtrDefs);

	//come back so that we compensate for the distance we would have traveled to shoot the flags
	robotDriver->driveRobot(-150, 50);

	//align to the cap 
	if(redAlliance){
		robotDriver->turnRobot(43, false);
	} else {
		robotDriver->turnRobot(45, true);
	}
	
	// Move forward so that when we bring the flipper down it is well over the peg of the
	// cap.
	robotDriver->driveRobot(420, 80);

	// Bring the flipper down
	flipperMove(mtrDefs, 2500, 80, -1);

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
	flipperMove(mtrDefs, 315, 127, 1);
	pros::Task::delay(200);

	// Bring flipper down so that when we move forward cap doesn't get stuck in intake
	robotDriver->driveRobot(-125, 80);
	flipperMove(mtrDefs, 3320, 127, -1);
	
	// Move forward a bit so that we are within range of top two middle flags

	robotDriver->driveRobot(475, 80);

	// Bring flipper back up with the hope that we flip the cap
	flipperMove(mtrDefs, 315, 90, 1);

	// Turn to make sure we can hit the flags
	if(redAlliance){
		robotDriver->turnRobot(10, false);
	} else{
		robotDriver->turnRobot(10, false);	
	}//turn right for blue as catapult is on the left side already 


	// Add a slight delay so that robot loads the balls properly
	pros::Task::delay(1500);

	// Shoot top two middle flags
	shootCatapult(mtrDefs);
}

void alignAndShootOurFlags(MotorDefs *mtrDefs, RobotDriver *robotDriver, bool redAlliance){
	if(redAlliance){
		robotDriver->driveRobot(225, 50);
	} else{
		robotDriver->driveRobot(75, 50);
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
	shootCatapult(mtrDefs);
	mtrDefs->intake_mtr->move(0);
}

void flipperLowFlag(MotorDefs *mtrDefs){
	mtrDefs->flipper_mtr->move_relative(-200, 200);
}

void toggleLowFlag(MotorDefs *mtrDefs, RobotDriver *robotDriver, bool redAlliance){
	if(redAlliance){
		robotDriver->turnRobot(12, true);
	} else {
		robotDriver->turnRobot(5, false);
	}
	robotDriver->smoothDrive(850, 127, 1);
	pros::Task::delay(100);
	robotDriver->smoothDrive(850, 127, -1);
}

void noAuton(){}