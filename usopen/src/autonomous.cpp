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

void pickUpBallFromPlatformAndBackToTile(MotorDefs *mtrDefs, bool redAlliance);
void pickupBallFromUnderCap(MotorDefs *mtrDefs, bool redAlliance);
void pickupBallsFromCapAndFlip(MotorDefs *mtrDefs, bool redAlliance);
void alignAndShootOurFlags(MotorDefs *mtrDefs, bool redAlliance);
void alignAndShootMiddleFlags(MotorDefs *mtrDefs, bool redAlliance);
void alignAndShootOpponentFlags(MotorDefs *mtrDefs, bool redAlliance);
void toggleLowFlag(MotorDefs *mtrDefs, bool redAlliance);

void pom(MotorDefs *mtrDefs, bool redAlliance);
void backAuton(MotorDefs *mtrDefs, bool redAlliance);
void noAuton();

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

void getPlatformBallAndBackToTile(MotorDefs *mtrDefs, bool redAlliance){
	smoothDrive(mtrDefs, 150, 80, 1);
	pros::Task::delay(100);
	if(redAlliance){
		turnRobot(mtrDefs, 45, false);
	} else {
		turnRobot(mtrDefs, 45, true);
	}
	pros::Task::delay(50);
	driveRobot(mtrDefs, 100, 50);
	mtrDefs->intake_mtr->move(127);
	mtrDefs->flipper_mtr->move(-40);
	pros::Task::delay(500);
	mtrDefs->flipper_mtr->move(0);
	pros::Task::delay(700);
	driveRobot(mtrDefs, -120, 50);
	mtrDefs->flipper_mtr->move(80);
	pros::Task::delay(50);
	driveRobot(mtrDefs, -150, 50);
	pros::Task::delay(50);
	mtrDefs->flipper_mtr->move(10);
	pros::Task::delay(100);
	if(redAlliance){
		turnRobot(mtrDefs, 45, true);
	} else {
		turnRobot(mtrDefs, 45, false);
	}
	pros::Task::delay(100);
	driveWithCoast(mtrDefs, 500, -80);
	driveWithCoast(mtrDefs, 150, -80);
}

void pom(MotorDefs *mtrDefs, bool redAlliance){
	getPlatformBallAndBackToTile(mtrDefs, redAlliance);
	alignAndShootOurFlags(mtrDefs, redAlliance);
	toggleLowFlag(mtrDefs, redAlliance);
}

void backAuton(MotorDefs *mtrDefs, bool redAlliance){

}


void noAuton(){}

void pickUpBallFromPlatformAndBackToTile(MotorDefs *mtrDefs, bool redAlliance){
	smoothDrive(mtrDefs, 150, 80, 1);
	pros::Task::delay(100);
	if(redAlliance){
		turnRobot(mtrDefs, 45, false);
	} else {
		turnRobot(mtrDefs, 45, true);
	}
	pros::Task::delay(50);
	driveRobot(mtrDefs, 100, 50);
	mtrDefs->intake_mtr->move(127);
	mtrDefs->flipper_mtr->move(-40);
	pros::Task::delay(500);
	mtrDefs->flipper_mtr->move(0);
	pros::Task::delay(700);
	driveRobot(mtrDefs, -120, 50);
	mtrDefs->flipper_mtr->move(80);
	pros::Task::delay(50);
	driveRobot(mtrDefs, -150, 50);
	pros::Task::delay(50);
	mtrDefs->flipper_mtr->move(10);
	pros::Task::delay(100);
	if(redAlliance){
		turnRobot(mtrDefs, 45, true);
	} else {
		turnRobot(mtrDefs, 45, false);
	}
	pros::Task::delay(100);
	driveWithCoast(mtrDefs, 500, -80);
	driveWithCoast(mtrDefs, 150, -30);	
}

void pickupBallFromUnderCap(MotorDefs *mtrDefs, bool redAlliance){

}

void pickupBallsFromCapAndFlip(MotorDefs *mtrDefs, bool redAlliance){

}

void alignAndShootOurFlags(MotorDefs *mtrDefs, bool redAlliance){
	pros::Task::delay(300);
	driveRobot(mtrDefs, 225, 50);
	pros::Task::delay(200);
	if(redAlliance){
		turnRobot(mtrDefs, 88, true);
	} else {
		turnRobot(mtrDefs, 90, false);
	}
	pros::Task::delay(100);
	driveRobot(mtrDefs, 150, 50);
	pros::Task::delay(500);
	shootCatapult(mtrDefs);
	mtrDefs->intake_mtr->move(0);
}

void alignAndShootMiddleFlags(MotorDefs *mtrDefs, bool redAlliance){

}

void alignAndShootOpponentFlags(MotorDefs *mtrDefs, bool redAlliance){

}

void toggleLowFlag(MotorDefs *mtrDefs, bool redAlliance){
	pros::Task::delay(500);
	if(redAlliance){
		turnRobot(mtrDefs, 11, true);
	} else {
		turnRobot(mtrDefs, 5, false);
	}
	pros::Task::delay(240);
	driveRobot(mtrDefs, 1000, 127);
	pros::Task::delay(100);
	driveRobot(mtrDefs, -800, 127);
}