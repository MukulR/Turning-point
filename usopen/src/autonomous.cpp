#include "autonselection.hpp"
#include "motordefs.hpp"

#define LEFT_TURN 0
#define RIGHT_TURN 1

//PID constants
#define KF 0
#define KP 1.0f
#define KI 0.001f
#define KD 0.1f

const float TURN_SCALE_FACTOR = 2.9444444;

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

void getPlatformBallAndAlignAgainstFence(MotorDefs *mtrDefs, bool redAlliance);
void pickupBallFromUnderCap(MotorDefs *mtrDefs, bool redAlliance);
void pickupBallsFromCapFlipAndShoot(MotorDefs *mtrDefs, bool redAlliance);
void alignAndShootOurFlags(MotorDefs *mtrDefs, bool redAlliance);
void alignAndShootMiddleFlags(MotorDefs *mtrDefs, bool redAlliance);
void alignAndShootOpponentFlags(MotorDefs *mtrDefs, bool redAlliance);
void toggleLowFlag(MotorDefs *mtrDefs, bool redAlliance);

void flipperLowFlag(MotorDefs *mtrDefs);

void pom(MotorDefs *mtrDefs, bool redAlliance);
void backAuton(MotorDefs *mtrDefs, bool redAlliance);
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

void initPIDVals(MotorDefs *mtrDefs){
	pros::motor_pid_s_t pid = pros::Motor::convert_pid(KF, KP, KI, KD);
	mtrDefs->left_mtr_f->set_pos_pid(pid);
	mtrDefs->left_mtr_b->set_pos_pid(pid);
	mtrDefs->right_mtr_f->set_pos_pid(pid);
	mtrDefs->right_mtr_b->set_pos_pid(pid);
	mtrDefs->middle_mtr->set_pos_pid(pid);

	mtrDefs->left_mtr_f->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	mtrDefs->left_mtr_b->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	mtrDefs->right_mtr_f->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	mtrDefs->right_mtr_b->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	mtrDefs->middle_mtr->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);

}

void waitForDriveCompletion(MotorDefs *mtrDefs){
	while((abs(mtrDefs->left_mtr_b->get_position() - mtrDefs->left_mtr_b->get_target_position()) + 
			abs(mtrDefs->left_mtr_f->get_position() - mtrDefs->left_mtr_f->get_target_position()) +
			abs(mtrDefs->right_mtr_b->get_position() - mtrDefs->right_mtr_b->get_target_position()) +
			abs(mtrDefs->right_mtr_f->get_position() - mtrDefs->right_mtr_f->get_target_position()) +
			abs(mtrDefs->middle_mtr->get_position() - mtrDefs->middle_mtr->get_target_position())) > 7 * 5 ) {
		pros::Task::delay(5);
	}
}

void waitForTurnCompletion(MotorDefs *mtrDefs) {
	while((abs(mtrDefs->left_mtr_b->get_position() - mtrDefs->left_mtr_b->get_target_position()) + 
			abs(mtrDefs->left_mtr_f->get_position() - mtrDefs->left_mtr_f->get_target_position()) +
			abs(mtrDefs->right_mtr_b->get_position() - mtrDefs->right_mtr_b->get_target_position()) +
			abs(mtrDefs->right_mtr_f->get_position() - mtrDefs->right_mtr_f->get_target_position())) > 7 * 4 ) {
		pros::Task::delay(5);			
	}
}

void driveRobot(MotorDefs *mtrDefs, double degrees, std::int32_t velocity){
	mtrDefs->left_mtr_f->move_relative(degrees, velocity);
	mtrDefs->left_mtr_b->move_relative(degrees, velocity);
	mtrDefs->right_mtr_f->move_relative(degrees, velocity);
	mtrDefs->right_mtr_b->move_relative(degrees, velocity);
	mtrDefs->middle_mtr->move_relative(degrees, velocity);
	waitForDriveCompletion(mtrDefs);
}

void driveWithCoast(MotorDefs *mtrDefs, int time, int power){
	mtrDefs->left_mtr_f->move(power);
	mtrDefs->left_mtr_b->move(power);
	mtrDefs->right_mtr_f->move(power);
	mtrDefs->right_mtr_b->move(power);
	mtrDefs->middle_mtr->move(power);
	pros::Task::delay(time);
	mtrDefs->left_mtr_f->move(0);
	mtrDefs->left_mtr_b->move(0);
	mtrDefs->right_mtr_f->move(0);
	mtrDefs->right_mtr_b->move(0);
	mtrDefs->middle_mtr->move(0);
}

void turnRobot(MotorDefs *mtrDefs, int unscaledDegs, bool left){
	float degrees = unscaledDegs * TURN_SCALE_FACTOR;
	mtrDefs->left_mtr_f->move(0);	
	mtrDefs->left_mtr_b->move(0);
	mtrDefs->right_mtr_f->move(0);
	mtrDefs->right_mtr_b->move(0);	
	if(left){
		mtrDefs->left_mtr_f->move_relative(-degrees, 65);	
		mtrDefs->left_mtr_b->move_relative(-degrees, 65);
		mtrDefs->right_mtr_f->move_relative(degrees, 65);
		mtrDefs->right_mtr_b->move_relative(degrees, 65);
	} else {
		mtrDefs->left_mtr_f->move_relative(degrees, 65);	
		mtrDefs->left_mtr_b->move_relative(degrees, 65);
		mtrDefs->right_mtr_f->move_relative(-degrees, 65);
		mtrDefs->right_mtr_b->move_relative(-degrees, 65);
	}
	waitForTurnCompletion(mtrDefs);
}

void smoothDrive(MotorDefs *mtrDefs, int degrees, int power, int direction){
	driveWithCoast(mtrDefs, 200, direction * 10);
	driveWithCoast(mtrDefs, 200, direction * 25);
	driveWithCoast(mtrDefs, 200, direction * 50);
	driveRobot(mtrDefs, direction * degrees, power);
}

void autonomous() {
	MotorDefs mtrDefs;
	initPIDVals(&mtrDefs);
	switch (autonSelected) {
		case 0:
			pom(&mtrDefs, redAlliance);
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
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

void getPlatformBallAndAlignAgainstFence(MotorDefs *mtrDefs, bool redAlliance){
	// Drive to the ball on platform
	smoothDrive(mtrDefs, 350, 80, 1);
	
	// Pickup the ball
	mtrDefs->intake_mtr->move(127);
	mtrDefs->flipper_mtr->move(-40);
	pros::Task::delay(350);
	driveRobot(mtrDefs, -50, 50);
	mtrDefs->flipper_mtr->move(0);
	pros::Task::delay(100);

	// Drive back to starting position
	smoothDrive(mtrDefs, 350, 80, -1);

	// Turn left if red alliance or right if blue alliance
	if(redAlliance){
		turnRobot(mtrDefs, 33, true);
	} else {
		turnRobot(mtrDefs, 33, false);
	}

	// Drive back so that the back of the robot aligns against the fence and bring up flipper
	driveWithCoast(mtrDefs, 800, -50);
	mtrDefs->flipper_mtr->move(110);
	pros::Task::delay(250);
	mtrDefs->flipper_mtr->move(5);
}

void pom(MotorDefs *mtrDefs, bool redAlliance){
	getPlatformBallAndAlignAgainstFence(mtrDefs, redAlliance);
	alignAndShootOurFlags(mtrDefs, redAlliance);
	//toggleLowFlag(mtrDefs, redAlliance);
	pickupBallsFromCapFlipAndShoot(mtrDefs, redAlliance);
}

void backAuton(MotorDefs *mtrDefs, bool redAlliance){

}


void noAuton(){}

void pickupBallFromUnderCap(MotorDefs *mtrDefs, bool redAlliance){

}

void flipperCapPos(MotorDefs *mtrDefs){
	mtrDefs->flipper_mtr->move_relative(-400, 200);
}

void pickupBallsFromCapFlipAndShoot(MotorDefs *mtrDefs, bool redAlliance){
	//bring catapult down to loading position
	mtrDefs->catapult_mtr->move(127);
	pros::Task::delay(100);
	while(bumper_auton.get_value()){
		pros::Task::delay(50);
	}
	mtrDefs->catapult_mtr->move(0);
	//pros::Task cataLoadTask (catapultLoad, mtrDefs, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "CatapultLoadTask");

	//come back so that we compensate for the distance we would have traveled to shoot the flags
	driveRobot(mtrDefs, -150, 50);

	//align to the cap 
	if(redAlliance){
		turnRobot(mtrDefs, 43, false);
	} else {
		turnRobot(mtrDefs, 43, true);
	}
	
	// Move forward so that when we bring the flipper down it is well over the peg of the
	// cap.
	driveRobot(mtrDefs, 460, 80);

	// Bring the flipper down
	flipperMove(mtrDefs, 2500, 80, -1);

	//block until catapultloadtask completes
	/*
	while(cataLoadTask.get_state() == pros::E_TASK_STATE_RUNNING){
		pros::Task::delay(10);
	}
	cataLoadTask.suspend();
	*/
	// Start the intake
	mtrDefs->intake_mtr->move(127);

	// Now drive back a bit so that cap bends towards the robot so that balls
	// fall into intake
	driveRobot(mtrDefs, -300, 100);

	// Bring the flipper up so that we let go of the cap
	flipperMove(mtrDefs, 315, 127, 1);
	pros::Task::delay(200);

	// Bring flipper down so that when we move forward cap doesn't get stuck in intake
	driveRobot(mtrDefs, -100, 80);
	flipperMove(mtrDefs, 3320, 127, -1);
	
	// Move forward a bit so that we are within range of top two middle flags
	driveRobot(mtrDefs, 460, 80);

	// Bring flipper back up with the hope that we flip the cap
	flipperMove(mtrDefs, 315, 90, 1);

	// Turn to make sure we can hit the flags
	if(redAlliance){
		turnRobot(mtrDefs, 8, false);
	} else{
		turnRobot(mtrDefs, 10, true);
	}

	// Add a slight delay so that robot loads the balls properly
	pros::Task::delay(1500);

	// Shoot top two middle flags
	shootCatapult(mtrDefs);
}

void alignAndShootOurFlags(MotorDefs *mtrDefs, bool redAlliance){
	driveRobot(mtrDefs, 225, 50);
	pros::Task::delay(100);
	if(redAlliance){
		turnRobot(mtrDefs, 88, true);
	} else {
		turnRobot(mtrDefs, 88, false);
	}
	pros::Task::delay(50);
	driveRobot(mtrDefs, 250, 50);
	pros::Task::delay(200);
	shootCatapult(mtrDefs);
	mtrDefs->intake_mtr->move(0);
}

void alignAndShootMiddleFlags(MotorDefs *mtrDefs, bool redAlliance){

}

void alignAndShootOpponentFlags(MotorDefs *mtrDefs, bool redAlliance){

}

void flipperLowFlag(MotorDefs *mtrDefs){
	mtrDefs->flipper_mtr->move_relative(-200, 200);
}

void toggleLowFlag(MotorDefs *mtrDefs, bool redAlliance){
	if(redAlliance){
		turnRobot(mtrDefs, 12, true);
	} else {
		turnRobot(mtrDefs, 5, false);
	}
	smoothDrive(mtrDefs, 850, 127, 1);
	pros::Task::delay(100);
	smoothDrive(mtrDefs, 850, 127, -1);
}