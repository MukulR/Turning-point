#include "uom.hpp"
#include "commons.hpp"
#include "motordefs.hpp"
#include "robot_driver.hpp"
#include "pros/adi.h"

//adi_gyro_t gyro = adi_gyro_init('C', 10);


UOM::UOM(MotorDefs *md, RobotDriver *rd, bool ra, Commons *ca, pros::ADIGyro *gy) {
    mtrDefs = md;
    robotDriver = rd;
    redAlliance = ra;
    commonAutons = ca;
    gyro = gy; 
}

UOM::~UOM() {}

void UOM::getBallFromUnderCapAndAlignAgainstFence() {
    // Hold the flipper up.
    mtrDefs->flipper_mtr->move(5);

	// Drive to the ball under the cap with the intake on
    mtrDefs->intake_mtr->move(127);
	robotDriver->smoothDrive(950, 120, 1);
    pros::Task::delay(500);

    // Drive back and align against fence
    robotDriver->smoothDrive(1000, 120, -1);
    robotDriver->driveWithCoast(750, -30);
}

void testGyro(){

}

void UOM::runAuton(){
    printf("Gyro: %lf\n", gyro->get_value());
    while(abs(gyro->get_value()) <= 900){
        printf("Gyro in loop: %lf\n", gyro->get_value());
        mtrDefs->left_mtr_f->move(50);
        mtrDefs->left_mtr_b->move(50);
        mtrDefs->right_mtr_f->move(-50);
        mtrDefs->right_mtr_f->move(-50);
    }
    mtrDefs->left_mtr_f->move(0);
    mtrDefs->left_mtr_b->move(0);
    mtrDefs->right_mtr_f->move(0);
    mtrDefs->right_mtr_f->move(0);
    /*
	getBallFromUnderCapAndAlignAgainstFence();
	commonAutons->alignAndShootOurFlags();
	//toggleLowFlag(mtrDefs, redAlliance);
	commonAutons->pickupBallsFromCapFlipAndShoot();*/
}
