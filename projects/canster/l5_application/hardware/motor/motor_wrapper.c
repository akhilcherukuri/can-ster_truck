#include "motor_wrapper.h"

#include "can_driver_node.h"
#include "esc.h"
#include "rpm.h"
#include "servo.h"

#include "project.h"

#include <stdbool.h>

void motor__init() {
  servo__steer_init();
  esc__init();
  rpm__init();
}

void motor__run_10hz(int callback_count) {
  // TODO Attach the motor steering decoder function here and check the decoded datatype from DBC
  if (mia_steering == true) {
    esc__neutral();
  } else {
    dbc_MOTOR_STEERING_s *decoded_steering_value_from_driver = can_driver__get_driver_steering();
    servo__steer_processor(decoded_steering_value_from_driver->MOTOR_STEERING_direction);
    if (callback_count % 10 == 0)
      rpm__calculate_speed_kph();

    // TODO Handle reverse cases and wrap this logic into a separate wrapper
    // if (decoded_steering_value_from_driver == 0)
    esc__forward_medium();
    // else
    //   esc__forward_slow();
  }
}