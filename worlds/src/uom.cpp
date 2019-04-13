#include "uom.hpp"
#include "commons.hpp"
#include "motordefs.hpp"
#include "robot_driver.hpp"

UOM::UOM(MotorDefs *md, RobotDriver *rd, bool ra, Commons *ca) {
    mtrDefs = md;
    robotDriver = rd;
    redAlliance = ra;
    commonAutons = ca;
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

void UOM::runAuton(){
	getBallFromUnderCapAndAlignAgainstFence();
	commonAutons->alignAndShootOurFlags();
	//toggleLowFlag(mtrDefs, redAlliance);
	commonAutons->pickupBallsFromCapFlipAndShoot();
}
