#include "autonselection.hpp"
#include "back.hpp"
#include "commons.hpp"
#include "motordefs.hpp"
#include "pom.hpp"
#include "robot_driver.hpp"
#include "uom.hpp"

/*
pros::ADIDigitalIn bumper_auton('C');
pros::ADIPotentiometer potentiometer('H');
*/

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
//void backAuton(MotorDefs *mtrDefs, RobotDriver *robotDriver, bool redAlliance);
void noAuton();

void autonomous() {
	MotorDefs mtrDefs;
	mtrDefs.initPIDVals();
	RobotDriver robotDriver(&mtrDefs);
	Commons commonAutons(&mtrDefs, &robotDriver, redAlliance);
	
	switch (autonSelected) {
		case 0:
			{
				POM pomAuton(&mtrDefs, &robotDriver, redAlliance, &commonAutons);
				pomAuton.runAuton();
			}
			break;
		case 1:
			{
				UOM uomAuton(&mtrDefs, &robotDriver, redAlliance, &commonAutons);
				uomAuton.runAuton();
			}
			break;
		case 2:
			{
				Back backAuton(&mtrDefs, &robotDriver, redAlliance, &commonAutons);
				backAuton.runAuton();
			}
			break;
		default:
			noAuton();
			break;
	}
}


void noAuton(){}