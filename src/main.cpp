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
#include "robodash/api.h" // IWYU pragma: keep


rd::Selector selector ({
	{"BluePosSixRing", BPosSixRing}
});

rd::Console console;



void initialize() {
	currState = Stow;
	chassis.calibrate();
	pros::lcd:: initialize();
	chassis.setPose(0, 0, 0); // Set the initial pose of the robots

	pros::Task screenTask([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            // delay to save resources
            pros::delay(50);
        }
    });
	
	pros::Task liftControlTask([]{
        while (true) {
            
            liftControl();
            pros::delay(10);
        }
    });
}

void disabled() {}


void competition_initialize() {}


void autonomous() {
	BPosSixRing();
}


void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	static bool HookState = false;
	bool hookToggleState = false; // Tracks whether the hook is spinning or stopped
 // loop forever
 while (true) {
       
         //Tank Drive
	int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
	int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

	chassis.tank(leftY, rightY);
// Controller Buttons
	if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
		descore();
	}
	if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
		nextState();
	}
	if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
		TipState();
	}

	
	if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
		Hook.move(127); // Spin forward when R2 is pressed
	} else 
		Hook.move(0); // Stop the hook when no button is pressed
	
	if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
		HoldState();
	}
 	if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        Mogo.set_value(false);
        } else 
        Mogo.set_value(true);
	
	
	if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)){
		static bool doinkerLState = false;
		doinkerLState = !doinkerLState;
		if (doinkerLState) {
			LDoinker.set_value(true);
		} else {
			LDoinker.set_value(false);
		}
	}
	if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)){
		static bool doinkerRState = false;
		doinkerRState = !doinkerRState;
		if (doinkerRState) {
			RDoinker.set_value(true);
		} else {
			RDoinker.set_value(false);
		}
	}

	pros::delay(10);
}
}