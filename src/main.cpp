// This file is where the main code is stored. It is the entry point for the program.
#pragma once 
#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "api.h" // IWYU pragma: keep 
#include "liblvgl/llemu.hpp"// IWYU pragma: keep
#include "autos.h" // IWYU pragma: keep
#include "devices.h"// IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp" 			// IWYU pragma: keep
#include "pros/abstract_motor.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"// IWYU pragma: keep
#include "pros/misc.h"
#include "pros/motors.hpp" // IWYU pragma: keep
#include "pros/rtos.hpp"
#include "liblvgl/core/lv_disp.h"// IWYU pragma: keep
#include "liblvgl/core/lv_obj_pos.h" // IWYU pragma: keep
#include "liblvgl/widgets/lv_img.h" // IWYU pragma: keep




void initialize() {
	pros::Task liftControlTask([]{
        while (true) {
            
            liftControl();
            pros::delay(10);
        }
    });
}


void disabled() {}


void competition_initialize() {}


void autonomous() {}


void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);


 // loop forever
 while (true) {
       
         //Tank Drive
	int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
	int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

	chassis.tank(leftY, rightY);
// Controller Buttons
	if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
		nextState();
	}


	if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
		if (currState == Tip) {
			currState = Stow;
		} else {
			currState = Tip;
		}
		updateTarget();
	}


	pros::delay(10);
}
}