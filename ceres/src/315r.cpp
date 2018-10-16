#include "main.h"




using namespace pros::literals;


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
/*
void initializeDrive();
void initializeCatapult();
void initializeIntake();

void control() {
    initializeDrive();
    initializeIntake();
    initializeCatapult();
}
*/

/*
void drive(void *param){   
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    pros::Motor left_mtr_f(1);
    pros::Motor left_mtr_b(2);
	pros::Motor right_mtr_f(3);
    pros::Motor right_mtr_b(4);
    while(true){
        left_mtr_f = master.get_analog(ANALOG_LEFT_Y);
        right_mtr_f = master.get_analog(ANALOG_LEFT_Y);
        left_mtr_b = master.get_analog(ANALOG_LEFT_Y);
        right_mtr_b = master.get_analog(ANALOG_LEFT_Y);
    
        //arcade turns assigned to right joystick (left / right)	
        if(master.get_analog(ANALOG_RIGHT_X) < 0){
            right_mtr_f = master.get_analog(ANALOG_RIGHT_X);
            right_mtr_b = master.get_analog(ANALOG_RIGHT_X);
            left_mtr_f = master.get_analog((ANALOG_RIGHT_X)) * -1;
            left_mtr_b = master.get_analog((ANALOG_RIGHT_X)) * -1;
        } 	else if(master.get_analog(ANALOG_RIGHT_X) > 0){
            right_mtr_f = master.get_analog(ANALOG_RIGHT_X) * -1;
            right_mtr_b = master.get_analog(ANALOG_RIGHT_X) * -1;
            left_mtr_f = master.get_analog((ANALOG_RIGHT_X));
            left_mtr_b = master.get_analog((ANALOG_RIGHT_X));
        }
        pros::Task::delay(10);
    }
}

void initializeDrive() {
    pros::Task driveTask(drive);
}

void catapult(){
    pros::Motor catapult_mtr(5);
    while(true){
        if(pros::E_CONTROLLER_DIGITAL_R2 == 1){
            while(pros::E_CONTROLLER_DIGITAL_R2 == 1){
                catapult_mtr.move(127);
            }
            catapult_mtr.move(0);
        }
        pros::delay(10);
    }            
}

void initializeCatapult(){
    pros::Task catapultTask(catapult, "PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT);    
}

void intake(){
    pros::Motor intake_mtr(6);
    while(true){
        if(pros::E_CONTROLLER_DIGITAL_R1 == 1){
            while(pros::E_CONTROLLER_DIGITAL_R1 == 1){
                intake_mtr.move(127);
            }
            intake_mtr.move(0);
        }
        pros::delay(10);
    }
}       

void initializeIntake(){
    pros::Task intakeTask(intake, "PROS,", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT);
}
*/