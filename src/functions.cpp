#include "robot.hpp"

// toggles the PTO mechanism
void toggle_pto() {
  // if controller's front right bumper is pressed, toggles the PTO mechanism
  if (prosController.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
    ptoActivated = !ptoActivated;
    rightPiston.set_value(ptoActivated);
    leftPiston.set_value(ptoActivated);
  }
}

/* utility function to shorten code
/ sets the speed of the pto motors.
*/
void set_ptom_speed(int speed) {
  lUFM = speed;
  rUFM = speed;
}

// the set of controls used when the PTO is activated
void pto_controls() {
  // if the PTO is activated, check for a controller input to move the motors
  // attache  d to the PTO, otherwise, don't move motors
  if (!ptoActivated) {
    if (prosController.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
      lUFM = 127;
      rUFM = 127;
    } else if (prosController.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
      lUFM = -127;
      rUFM = -127;
    } else {
      lUFM = 0;
      rUFM = 0;
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