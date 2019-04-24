#include "motordefs.hpp"
#include "robot_driver.hpp"

const float TURN_SCALE_FACTOR = 3.0;
const int NUM_TURN_MOTORS= 4;
const int NUM_DRIVE_MOTORS = 5;

RobotDriver::RobotDriver(MotorDefs *motorDefs, pros::ADIGyro *gy) {
    mtrDefs = motorDefs;
	gyro = gy;
}

RobotDriver::~RobotDriver() {}

void RobotDriver::waitForDriveCompletion() {
	while((abs(mtrDefs->left_mtr_b->get_position() - mtrDefs->left_mtr_b->get_target_position()) + 
			abs(mtrDefs->left_mtr_f->get_position() - mtrDefs->left_mtr_f->get_target_position()) +
			abs(mtrDefs->right_mtr_b->get_position() - mtrDefs->right_mtr_b->get_target_position()) +
			abs(mtrDefs->right_mtr_f->get_position() - mtrDefs->right_mtr_f->get_target_position()) +
			abs(mtrDefs->middle_mtr->get_position() - mtrDefs->middle_mtr->get_target_position())) > 7 * NUM_DRIVE_MOTORS ) {
		pros::Task::delay(5);
	}

}

void RobotDriver::waitForTurnCompletion() {
    while((abs(mtrDefs->left_mtr_b->get_position() - mtrDefs->left_mtr_b->get_target_position()) + 
                abs(mtrDefs->left_mtr_f->get_position() - mtrDefs->left_mtr_f->get_target_position()) +
                abs(mtrDefs->right_mtr_b->get_position() - mtrDefs->right_mtr_b->get_target_position()) +
                abs(mtrDefs->right_mtr_f->get_position() - mtrDefs->right_mtr_f->get_target_position())) > 7 * NUM_TURN_MOTORS ) {
            pros::Task::delay(5);			
        }
}

void RobotDriver::driveRobot(double degrees, std::int32_t velocity){
	mtrDefs->left_mtr_f->move_relative(degrees, velocity);
	mtrDefs->left_mtr_b->move_relative(degrees, velocity);
	mtrDefs->right_mtr_f->move_relative(degrees, velocity);
	mtrDefs->right_mtr_b->move_relative(degrees, velocity);
	mtrDefs->middle_mtr->move_relative(degrees, velocity);
	waitForDriveCompletion();
}

void RobotDriver::driveWithCoast(int time, int power){
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


void RobotDriver::verifyTurns(MotorDefs *mtrDefs, int degrees, int direction){
	std::cout << "Gyro val: " << gyro->get_value() << "\n";
	int remainingDegrees = (abs(degrees * 10) - abs(gyro->get_value())) / 10;
	remainingDegrees *= TURN_SCALE_FACTOR;

	std::cout << "Remaining Degrees: " << remainingDegrees << "\n";
	
	if (remainingDegrees > 0) {
		if(direction == 1) { // Turning left
			if (remainingDegrees > 0) { //This is the case where we have turned less
				mtrDefs->left_mtr_f->move_relative(-remainingDegrees, 65);	
				mtrDefs->left_mtr_b->move_relative(-remainingDegrees, 65);
				mtrDefs->right_mtr_f->move_relative(remainingDegrees, 65);
				mtrDefs->right_mtr_b->move_relative(remainingDegrees, 65);
			} else { // This is the case where we have turned more
				mtrDefs->left_mtr_f->move_relative(remainingDegrees, 65);	
				mtrDefs->left_mtr_b->move_relative(remainingDegrees, 65);
				mtrDefs->right_mtr_f->move_relative(-remainingDegrees, 65);
				mtrDefs->right_mtr_b->move_relative(-remainingDegrees, 65);
			}
		} else {
			if (remainingDegrees > 0) {
				mtrDefs->left_mtr_f->move_relative(remainingDegrees, 65);	
				mtrDefs->left_mtr_b->move_relative(remainingDegrees, 65);
				mtrDefs->right_mtr_f->move_relative(-remainingDegrees, 65);
				mtrDefs->right_mtr_b->move_relative(-remainingDegrees, 65);
			} else {
				mtrDefs->left_mtr_f->move_relative(-remainingDegrees, 65);	
				mtrDefs->left_mtr_b->move_relative(-remainingDegrees, 65);
				mtrDefs->right_mtr_f->move_relative(remainingDegrees, 65);
				mtrDefs->right_mtr_b->move_relative(remainingDegrees, 65);
			}
		}
		//make sure drive doesnt coast after verifying turn with gyroscope
		mtrDefs->left_mtr_f->move_relative(0, 100);	
		mtrDefs->left_mtr_b->move_relative(0, 100);
		mtrDefs->right_mtr_f->move_relative(0, 100);
		mtrDefs->right_mtr_b->move_relative(0, 100);
	}
}

void RobotDriver::turnRobot(int unscaledDegs, bool left){
	float degrees = unscaledDegs * TURN_SCALE_FACTOR;
	mtrDefs->left_mtr_f->move(0);	
	mtrDefs->left_mtr_b->move(0);
	mtrDefs->right_mtr_f->move(0);
	mtrDefs->right_mtr_b->move(0);
	gyro->reset();
	printf("Gyro val after reset: %d\n", gyro->get_value());

	if(left){
		mtrDefs->left_mtr_f->move_relative(-degrees, 65);	
		mtrDefs->left_mtr_b->move_relative(-degrees, 65);
		mtrDefs->right_mtr_f->move_relative(degrees, 65);
		mtrDefs->right_mtr_b->move_relative(degrees, 65);
		waitForTurnCompletion();
		verifyTurns(mtrDefs, unscaledDegs, -1);
	} else {
		mtrDefs->left_mtr_f->move_relative(degrees, 65);	
		mtrDefs->left_mtr_b->move_relative(degrees, 65);
		mtrDefs->right_mtr_f->move_relative(-degrees, 65);
		mtrDefs->right_mtr_b->move_relative(-degrees, 65);
		waitForTurnCompletion();
		verifyTurns(mtrDefs, degrees, -1);
	}
}

void RobotDriver::smoothDrive(int degrees, int power, int direction){
	driveWithCoast(200, direction * 10);
	driveWithCoast(200, direction * 25);
	driveWithCoast(200, direction * 50);
	driveRobot(direction * degrees, power);
}