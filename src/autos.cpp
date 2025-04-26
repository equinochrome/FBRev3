#include "devices.h"    // IWYU pragma: keep
#include "autos.h"
#include "lemlib/chassis/chassis.hpp"       // IWYU pragma: keep
#include "pros/motors.h" // IWYU pragma: keep
#include "pros/rtos.hpp" // IWYU pragma: keep 
#include "devices.h" // IWYU pragma: keep

//Lift State
void HoldState() {
    if (currState == Stow) {
        currState = Hold;
    } else if (currState == Prime) {
        currState = Hold;
    } else if (currState == Score) {
        currState = Hold;
    } else if (currState == Tip) {
        currState = Hold;
    } else if (currState == Hold) {
        currState = Stow;
    } else if (currState == Descore) {
        currState = Hold;
    }
    updateTarget();
}
void nextState() {
    if (currState == Stow) {
        currState = Prime;
    } else if (currState == Prime) {
        currState = Score;
    } else if (currState == Hold) {
        currState = Score;
    } else if (currState == Score) {
        currState = Stow;
    } else if (currState == Tip) {
        currState = Stow;
    } else if (currState == Descore) {
        currState = Stow;
      }

updateTarget();
}
void descore() {
    if (currState == Stow) {
        currState = Descore;
    } else if (currState == Prime) {
        currState = Descore;
    } else if (currState == Score) {
        currState = Descore;
    } else if (currState == Tip) {
        currState = Descore;
    } else if (currState == Hold) {
        currState = Descore;
    } else if (currState == Descore) {
        currState = Stow;
    }
    updateTarget();
}
void TipState() {
    if (currState == Stow) {
        currState = Tip;
    } else if (currState == Score) {
        currState = Tip;
    } else if (currState == Prime) {
        currState = Tip;
    } else if (currState == Hold) {
        currState = Tip;
    }  else if (currState == Tip) {
        currState = Stow;
    }
    if (currState == Descore) {
        currState = Tip;
    }
    updateTarget();

}

void updateTarget() {
    switch (currState) {
        case Stow:
            target = 293; // Stow position
            break;
        case Prime:
            target = 3380; // Prime position
            break;
        case Hold:
            target = 7193; // Hold position
            break;
        case Descore:
            target = 15893; // Descore position
            break;
        case Score:
            target = 16893; // Score position
            break;
        
        case Tip:
            target = 24193; // Tip position
            break;
    }
}

double prevError = 0;

void liftControl() {
    double kp = .015;
    double kd = 0.0;
    double error = target - rotationSensor3.get_position();
    double deriv = error - prevError;
    prevError = error;
    double velocity = kp * error + deriv * kd;
    LB.move(velocity);
}
int pastVoltage = 0;

void ColorSort(){
    
    color.set_led_pwm(100);
    pastVoltage = Hook.get_voltage();

    if (BlueTeam) {
        if(color.get_hue() < 15){

            Hook.move(0);
            pros::delay(300);
            Hook.move(pastVoltage);
        }
    } else

    if (RedTeam) {
        if(color.get_hue() > 200){
            pros::delay(105);
            Hook.move(0);
            pros::delay(300);
            Hook.move(pastVoltage);
        }
    }
}



//Autos

void BPosRushQuals(){
    // 5mogo+1alliance
    // set up
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
    LB.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    BlueTeam = true;
    RedTeam = false;
    chassis.setPose(-50.400158, 42.325857, 77.810095); // Set the initial pose of the robot
    Mogo.set_value(false);

    // rush for middle ring 4 stack
    LDoinker.set_value(true);
    Hook.move(127);
    chassis.moveToPoint(-7.794155, 54.784473, 1000);

};

void SixMogoRush(){
    // 6mogorush+1alliance
    // set up
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
    LB.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    BlueTeam = true;
    RedTeam = false;
    chassis.setPose(-49.472337, 40.987507, 68.584198); // Set the initial pose of the robot
    Mogo.set_value(false);

    // rush for middle ring 4 stack
    LDoinker.set_value(true);
    Hook.move(50);
    chassis.moveToPoint(-11, 54.5, 1000, {.minSpeed=100});
	// grab goal
	pros::delay(200);
	chassis.moveToPoint(-26, 26, 1000, {.forwards=false, .maxSpeed=80}, false);
	LDoinker.set_value(false); // let go of rushed ring
	Mogo.set_value(true);
	// score rushed rings + single stack
	chassis.moveToPoint(-24, 64, 3000, {.maxSpeed=70}, true);
	pros::delay(100);
	Hook.move(127);
	// score 2 rings from corner stack
	chassis.moveToPose(-69, 84, -40, 2000, {.lead=.4, .minSpeed=65});
	pros::delay(400);
	nextState();
	nextState();
	chassis.moveToPoint(-61, 71, 400, {.forwards=false}, false);
	IntakePiston.set_value(true);
	chassis.moveToPoint(-69, 84, 1000, {}, false);
	IntakePiston.set_value(false);
	chassis.moveToPoint(-51, 58, 1000, {.forwards=false}, false);
	chassis.turnToHeading(180, 700, {}, false);
	nextState();
	chassis.moveToPoint(-48, 28, 1000, {.maxSpeed=80}, false);
	pros::delay(500);
	IntakePiston.set_value(true);
	pros::delay(100);
	nextState();
	chassis.moveToPoint(-48, 6, 1000, {}, false);
	IntakePiston.set_value(false);
	chassis.moveToPoint(-48, 9.5, 1000, {.forwards=false}, false);
	RDoinker.set_value(true);
	chassis.turnToHeading(270, 700, {}, false);
	RDoinker.set_value(false);
	chassis.moveToPoint(-80, 9.5, 500, {}, false);
	chassis.setPose(0, 0, 270);  // reset pose for relative movement
	// chassis.moveToPoint(-9, 0, 1000, {.forwards=false}, false);
}

void BPosSixRing(){
    //6Ring Blue
    // setup
    chassis.setPose(51, -24, 90);
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
    LB.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    BlueTeam = true;
    RedTeam = false;
    Mogo.set_value(false);

    chassis.moveToPoint(25, -24, 1000, {.forwards=false, .maxSpeed=60}, false);
    Mogo.set_value(true);
    pros::delay(50);
    Hook.move(127);
    chassis.turnToHeading(-45, 700, {}, false);
    Hook.move(40);
    chassis.moveToPoint(5, -7, 1000, {.maxSpeed = 65}, false);
    LDoinker.set_value(true);
    pros::delay(50);
    chassis.swingToHeading(-67.5, lemlib::DriveSide::LEFT, 700, {.minSpeed = 30}, false);
    RDoinker.set_value(true);
    pros::delay(50);
    Hook.move(-50);
    chassis.moveToPose(40, -32, -90, 5000, {.forwards=false, .lead = .21, .maxSpeed=75, .minSpeed = 35}, false);
    LDoinker.set_value(false); // this line and below not downloaded as of 4/25
    RDoinker.set_value(false);
    pros::delay(50);
    chassis.moveToPoint(30, -24, 900);

}