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
            target = 0; // Stow position
            break;
        case Prime:
            target = 3190; // Prime position
            break;
        case Hold:
            target = 7000; // Hold position
            break;
        case Descore:
            target = 15700; // Descore position
            break;
        case Score:
            target = 16700; // Score position
            break;
        
        case Tip:
            target = 24000; // Tip position
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

void Base6RingPos(){
    
};