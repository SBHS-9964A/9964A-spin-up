#include "pros/adi.h"
#include "robot.hpp"

// toggles the PTO mechanism
void toggle_pto() {
  // if controller's front right bumper is pressed, toggles the PTO mechanism
  if (prosController.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
    ptoActivated = !ptoActivated;
    rightPiston.set_value(ptoActivated);
    leftPiston.set_value(ptoActivated);
  }
}

/* utility function to shorten code
 * sets the speed of the pto motors.
 * @param speed The speed that the PTO motors will be set to.
 * 127 to -127
 */
void set_ptom_speed(int speed) {
  lUFM = speed;
  rUFM = speed;
}
void update_controller() {
  std::string on = "t";
  std::string off = "f";
  const char *onValue = on.c_str();
  const char *offValue = off.c_str();
  if (ptoActivated) {
    prosController.print(0, 3, onValue);
  } else {
    prosController.print(0, 3, offValue);
  }
}
// the set of controls used when the PTO is activated
void pto_controls() {
  // if the PTO is activated, check for a controller input to move the motors
  // attached to the PTO, otherwise, don't move motors

  // Input logic
  if (prosController.get_digital_new_press(
          pros::E_CONTROLLER_DIGITAL_R2)) { // check if r2 has been pressed
    windingBack = true; // update value, user wants to windback catapult
  }
  if (prosController.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
    intakeActivated = !intakeActivated;
  }
  pros::delay(300);
  // activation logic
  if (!ptoActivated) { // check if PTO is engaged into windback/intake mode
    if (windingBack &&
        !windbackLimit
             .get_new_press()) { // check if the user wants to windback the
                                 // catapult and if the catapult cant be wound
      set_ptom_speed(127);       // windback the catapult
    } else {
      if (intakeActivated) {
        set_ptom_speed(-127);
      } else {
        set_ptom_speed(0);
      }
      windingBack = false;
    }
  }
}

void extension() {
  if (prosController.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
    jerry.set_value(!extensionActivated);
    extensionActivated = !extensionActivated;
  }
}

// only rumbles once to not annoy the driver
bool firstRumble = true;

// array of motors to iterate through.
pros::Motor motors[8] = {rLFM, rLBM, rUFM, rUBM, lLFM, lLBM, lUFM, lUBM};

void temp_rumble() {
  if (firstRumble) {
    for (int i = 0; i < sizeof(motors); i++) {
      if (motors[i].get_temperature() > 40) {
        firstRumble = false;
        prosController.rumble("...");
        break;
      }
    }
  }
}
// function to actuate drivetrain. uses 6 vs 8 motors depending on status of
// pto.
void update_drivetrain() {
  /* if (ptoActivated) {
     lUFM = prosController.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
     rUFM = prosController.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
   } */
  prosLDM = prosController.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
  prosRDM = prosController.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
}

// function to actuate the roller mech. uses up and down arrows to spin the
// roller mech
void roll_roller() {
  lUFM = 100 * (prosController.get_digital(pros::E_CONTROLLER_DIGITAL_UP) -
                prosController.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN));
}