#include "main.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor left_mtr_f(1);
pros::Motor right_mtr_f(4);
pros::Motor left_mtr_m(19);
pros::Motor right_mtr_m(14);
pros::Motor left_mtr_b(11);
pros::Motor right_mtr_b(2);
pros::Motor intake_mtr(6);

/*
void stopAll(){
	left_mtr_f.move(0);
	right_mtr_f.move(0);
	left_mtr_b.move(0);
	right_mtr_b.move(0);
	intake_mtr.move(0);
}
*/
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
void opcontrol() {
	while (true) {
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
			left_mtr_f.move(127);
			pros::Task::delay(2000);
			left_mtr_f.move(0);
		} else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A)){
			right_mtr_f.move(-127);
			pros::Task::delay(2000);
			right_mtr_f.move(0);
		} else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
			right_mtr_b.move(-127);
			pros::Task::delay(2000);
			right_mtr_b.move(0);
		} else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)){
			left_mtr_b.move(127);
			pros::Task::delay(2000);
			left_mtr_b.move(0);
		} else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)){
			intake_mtr.move(127);
			pros::Task::delay(2000);
			intake_mtr.move(0);
		} else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)){
			left_mtr_m.move(127);
			pros::Task::delay(2000);
			left_mtr_m.move(0);
		} else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)){
			right_mtr_m.move(127);
			pros::Task::delay(2000);
			right_mtr_m.move(0);
		} 


	}
}
