#include "main.h"
#include "motordefs.hpp"
#include "autonselection.h"

int autonSelected;
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		switch (autonSelected){
			case 0:
				pros::lcd::set_text(2, "Red Auton Selected!");
				break;
			case 1:
				pros::lcd::set_text(2, "Red with Park Auton Selected!");
				break;
			case 2:
				pros::lcd::set_text(2, "Blue Auton Selected!");
				break;
			case 3:
				pros::lcd::set_text(2, "Blue with Park Auton Selected!");
				break;
			case 4:
				pros::lcd::set_text(2, "Back Auton Selected!");
				break;
			case 5:
				pros::lcd::set_text(2, "Back with Park Auton Selected!");
				break;
			default:
				pros::lcd::set_text(2, "Auton Diasbled!");
				break;
		}
	}
}

void on_right_button(){
	static bool pressed = false;
	static int numPresses = 0;
	pressed = !pressed;
	if (pressed){
		switch(numPresses){
			case 0:
				pros::lcd::set_text(2, "Red Auton");
				autonSelected = 0;
				break;
			case 1:
				pros::lcd::set_text(2, "Red With Park Auton");
				autonSelected = 1;
				break;
			case 2: 
				pros::lcd::set_text(2, "Blue Auton");
				autonSelected = 2;
				break;
			case 3: 
				pros::lcd::set_text(2, "Blue With Park Auton");
				autonSelected = 3;
				break;
			case 4:
				pros::lcd::set_text(2, "Back Auton");
				autonSelected = 4;
				break;
			case 5:
				pros::lcd::set_text(2, "Back With Park Auton");
				autonSelected = 5;
				break;
			case 6:
				pros::lcd::set_text(2, "Auton Disabled");
				autonSelected = 6;
				break;
		}
		numPresses++;
		if(numPresses > 6){
			numPresses = 0;
		}
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "315R Paradigm          Connect to Comp Switch to choose auton!");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	pros::lcd::set_text(1, "Right Button For Auton Options");
	pros::lcd::set_text(2, "Center Button For Auton Selection");

	// Register functions for right button press and center button press
	pros::lcd::register_btn1_cb(on_center_button);
	pros::lcd::register_btn2_cb(on_right_button);
}

