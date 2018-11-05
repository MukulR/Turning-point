#include "main.h"
#include "motordefs.hpp"

#include "okapi/api.hpp"
using namespace okapi;

MotorDefs mtrDefs;

pros::Controller master(pros::E_CONTROLLER_MASTER);

void driveWithOkapi(void* param){
	Motor left_mtr_f(1, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
	Motor right_mtr_f(2, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
	Motor left_mtr_b(3, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
	Motor right_mtr_b(4, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
	
	left_mtr_f.setVoltageLimit(12000);
	left_mtr_b.setVoltageLimit(12000);
	right_mtr_f.setVoltageLimit(12000);
	right_mtr_b.setVoltageLimit(12000);

	while(true){
		int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int forward = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		float scaledTurn = ((turn * 100) * 0.75) / 100;
		float leftMtrVals = (forward + scaledTurn); //temp original forward + turn
		float rightMtrVals = -(forward - scaledTurn); //temp original -(forward - scaled turn)
		if(leftMtrVals > 127){
			leftMtrVals = 127;
		}
		if(leftMtrVals < -127){
			leftMtrVals = -127;
		}
		if(rightMtrVals > 127){
			rightMtrVals = 127;
		}
		if(rightMtrVals < -127){
			rightMtrVals = -127;
		}
		left_mtr_b.move(leftMtrVals);
		left_mtr_f.move(leftMtrVals);
		right_mtr_b.move(rightMtrVals);
		right_mtr_f.move(rightMtrVals);
		// The below delay is required for tasks to work in PROS.
		pros::Task::delay(10);
	}
}

void catapult(void* param){
    while(true){
        if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
            while(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
                mtrDefs.catapult_mtr->move(127);
            }
            mtrDefs.catapult_mtr->move(0);
        }
        pros::Task::delay(10);
    }
}


void intake(void* param){
	static bool intakeStarted = false;
    while(true){
        if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
			if(!intakeStarted){
				mtrDefs.intake_mtr->move(127);	
				intakeStarted = true;
			} else if(intakeStarted){
				mtrDefs.intake_mtr->move(0);
				intakeStarted = false;
			}
			pros::Task::delay(300);
		}
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){	
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
				mtrDefs.intake_mtr->move(-127);
			}
			mtrDefs.intake_mtr->move(0);
		}
        pros::Task::delay(10);
    }
}       

void brake(void* param){
	while(true){
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
				mtrDefs.brake_mtr->move(-127);
			}
			mtrDefs.brake_mtr->move(0);
		}
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
				mtrDefs.brake_mtr->move(127);
			}
			mtrDefs.brake_mtr->move(15);
		}
		pros::Task::delay(10);
	}
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
//task starter function (called by competition)


void opcontrol() {
	pros::Task driveOkapiTask(driveWithOkapi);
	pros::Task catapultTask(catapult);
	pros::Task intakeTask(intake);
	pros::Task brakeTask(brake);
}