#pragma once

#include "lemlib/chassis/chassis.hpp" // IWYU pragma: keep
#include "pros/adi.hpp" // IWYU pragma: keep
#include "pros/distance.hpp" // IWYU pragma: keep
#include "pros/imu.hpp" // IWYU pragma: keep
#include "pros/misc.hpp" // IWYU pragma: keep
#include "pros/motor_group.hpp" // IWYU pragma: keep
#include "pros/motors.hpp"   // IWYU pragma: keep
#include "pros/optical.hpp"     // IWYU pragma: keep
#include "pros/rotation.hpp" // IWYU pragma: keep
#include "cmath"

enum LiftState {
    Stow,
    Prime,
    Score,
    Tip,
};
static LiftState currState = Stow;
static int target = 0;


// Controller


static pros::Controller controller(pros::E_CONTROLLER_MASTER);

static pros::MotorGroup left_motors(	{-16,-11,14}, pros::MotorGearset::blue);
static pros::MotorGroup right_motors({12,20,-15}, pros::MotorGearset::blue);


static pros::Motor Hook(-10, pros::MotorGearset::blue);
static pros::Motor Intake(-2, pros::MotorGearset::blue);
static pros::Motor LB (-13, pros::MotorGearset::green);

static pros::adi::DigitalOut IntakePiston('C');
static pros::adi::DigitalOut Doinker('D'); 
static pros::adi::DigitalOut Mogo('E'); 
static pros::adi::DigitalOut Tipper('B');
static pros::Optical color(18);
static pros::Distance rightDistance(9);
static pros::adi::DigitalOut Clamp('F');

// drivetrain settings
static lemlib::Drivetrain drivetrain(&left_motors, // left motor group
                              &right_motors, // right motor group
                              11.384000, // 12.85 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
                              450, // drivetrain rpm is 450
                              2 // all omni
);


// create an imu on port 1
static pros::Imu imu(4);


// create a v5 rotation sensor on port 7
static pros::Rotation rotation_sensor(-1);
static pros::Rotation rotation_sensor2(19);
static pros::Rotation rotationSensor3(3);
// create a distance sensor for lb distance
static pros::Distance color_dist(7);
static pros::adi::DigitalIn limit('A');


// replace 1 with the port the rotation sensor is connected to
static pros::Rotation vertical_sensor(-1);
static pros::Rotation horiz_sensor(-19);

// vertical tracking wheel
static lemlib::TrackingWheel vertical_tracking_wheel(&vertical_sensor, lemlib::Omniwheel::NEW_2, 0.176743);
//horizontal tracker
static lemlib::TrackingWheel horiz_tracker(&horiz_sensor, lemlib::Omniwheel::NEW_2, -2.875);

static lemlib::OdomSensors sensors(
                            &vertical_tracking_wheel, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            &horiz_tracker, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);
// lateral PID controller
static lemlib::ControllerSettings lateral_controller(
                                              10, // proportional gain (kP)
                                              0.01, // integral gain (kI)
                                              55 , // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular PID controller
static lemlib::ControllerSettings angular_controller(1.5, // proportional gain (kP)
                                            .1, // integral gain (kI)
                                              10.5, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);
// input curve for throttle input during driver control
static lemlib::ExpoDriveCurve throttle_curve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
static lemlib::ExpoDriveCurve steer_curve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
static lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors // odometry sensors
);



// Other stuff
static bool BlueTeam = false;
static bool RedTeam = false;
static bool Auto = false;
