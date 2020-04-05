#include "motor_wrapper.h"

#include "esc.h"
#include "rpm.h"
#include "servo.h"

void motor__init() {
  servo__steer_init();
  esc__init();
  rpm__init();
}

void motor__run_10hz(int callback_count) {
  servo__steer_processor(0); // Steering Value from Driver
  if (callback_count % 10 == 0)
    rpm__calculate_speed();
  esc__forward_slow();
}