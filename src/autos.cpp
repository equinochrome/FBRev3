#include "devices.h"    // IWYU pragma: keep
#include "autos.h"
#include "lemlib/chassis/chassis.hpp"       // IWYU pragma: keep
#include "pros/motors.h" // IWYU pragma: keep
#include "pros/rtos.hpp" // IWYU pragma: keep 
#include "devices.h" // IWYU pragma: keep




//Lift State
void nextState() {
    if (currState == Stow) {
        currState = Prime;
    } else if (currState == Prime) {
        currState = Score;
    } else if (currState == Score) {
        currState = Stow;
    }
    updateTarget();
}

void updateTarget() {
    switch (currState) {
        case Stow:
            target = 0; // Stow position
            break;
        case Prime:
            target = 3200; // Prime position
            break;
        case Score:
            target = 16100; // Score position
            break;
        case Tip:
            target = 32000; // Tip position
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

//Autos