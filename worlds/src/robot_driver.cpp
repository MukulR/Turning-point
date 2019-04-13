#include "motordefs.hpp"
#include "robot_driver.hpp"

const float TURN_SCALE_FACTOR = 2.9444444;

pros::ADIGyro gyro('C');

RobotDriver::RobotDriver(MotorDefs *motorDefs) {
    mtrDefs = motorDefs;
}

RobotDriver::~RobotDriver() {}

void RobotDriver::waitForDriveCompletion() {
	while((abs(mtrDefs->left_mtr_b->get_position() - mtrDefs->left_mtr_b->get_target_position()) + 
			abs(mtrDefs->left_mtr_f->get_position() - mtrDefs->left_mtr_f->get_target_position()) +
			abs(mtrDefs->right_mtr_b->get_position() - mtrDefs->right_mtr_b->get_target_position()) +
			abs(mtrDefs->right_mtr_f->get_position() - mtrDefs->right_mtr_f->get_target_position()) +
			abs(mtrDefs->middle_mtr->get_position() - mtrDefs->middle_mtr->get_target_position())) > 7 * 5 ) {
		pros::Task::delay(5);
	}

}

void RobotDriver::waitForTurnCompletion() {
    while((abs(mtrDefs->left_mtr_b->get_position() - mtrDefs->left_mtr_b->get_target_position()) + 
                abs(mtrDefs->left_mtr_f->get_position() - mtrDefs->left_mtr_f->get_target_position()) +
                abs(mtrDefs->right_mtr_b->get_position() - mtrDefs->right_mtr_b->get_target_position()) +
                abs(mtrDefs->right_mtr_f->get_position() - mtrDefs->right_mtr_f->get_target_position())) > 7 * 4 ) {
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

void RobotDriver::turnRobot(int unscaledDegs, bool left){
	float degrees = unscaledDegs * TURN_SCALE_FACTOR;
	mtrDefs->left_mtr_f->move(0);	
	mtrDefs->left_mtr_b->move(0);
	mtrDefs->right_mtr_f->move(0);
	mtrDefs->right_mtr_b->move(0);
	gyro.reset();	
	if(left){
		mtrDefs->left_mtr_f->move_relative(-degrees, 65);	
		mtrDefs->left_mtr_b->move_relative(-degrees, 65);
		mtrDefs->right_mtr_f->move_relative(degrees, 65);
		mtrDefs->right_mtr_b->move_relative(degrees, 65);
		gyro.get_value();
	} else {
		mtrDefs->left_mtr_f->move_relative(degrees, 65);	
		mtrDefs->left_mtr_b->move_relative(degrees, 65);
		mtrDefs->right_mtr_f->move_relative(-degrees, 65);
		mtrDefs->right_mtr_b->move_relative(-degrees, 65);
	}
	waitForTurnCompletion();
}

void RobotDriver::smoothDrive(int degrees, int power, int direction){
	driveWithCoast(200, direction * 10);
	driveWithCoast(200, direction * 25);
	driveWithCoast(200, direction * 50);
	driveRobot(direction * degrees, power);
}