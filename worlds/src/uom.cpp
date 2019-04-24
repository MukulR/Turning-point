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
    // bring the flipper to cap position
    pros::Task flipperTask(Commons::flipperCapPos, mtrDefs);
	// Drive to the ball under the cap with the intake on
    mtrDefs->intake_mtr->move(127);
	robotDriver->smoothDrive(950, 120, 1);
    pros::Task::delay(500);

    // Drive back and align against fence
    robotDriver->smoothDrive(1000, 120, -1);
    //bring flipper back up
    pros::Task flipperUpTask(Commons::flipperUp, mtrDefs);
    robotDriver->driveWithCoast(750, -30);
}

void testGyro(){

}

void UOM::runAuton(){
	getBallFromUnderCapAndAlignAgainstFence();
	commonAutons->alignAndShootOurFlags();
	//toggleLowFlag(mtrDefs, redAlliance);
	commonAutons->pickupBallsFromCapFlipAndShoot();
}
