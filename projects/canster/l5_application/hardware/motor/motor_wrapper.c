#include "motor_wrapper.h"

#include "can_driver_node.h"
#include "esc.h"
#include "rpm.h"
#include "servo.h"

#include "project.h"

#include <stdbool.h>

#include "pwm1.h"

#define DEBUG_STATE_MACHINE 0

/**
 * STATIC VARIABLES
 */
static current_motor_state_e last_state = MOTOR_INIT;
static uint32_t entry_time_of_state_change;
static bool state_change = false;

static current_motor_state_e current_motor_state;
dbc_MOTOR_SPEED_s decoded_speed_value_from_driver;

void motor__init() {
  servo__steer_init();
  esc__init();
  rpm__init();
}
/**
 * STATIC FUNCTIONS
 */
static void motor__run_state_machine_10hz(current_motor_state_e, uint32_t);

/**
 * NON-STATIC FUNCTION DEFINITIONS
 */
void motor__run_10hz(uint32_t callback_count) {
  bool mia_steering = get_mia_steering();
  if (mia_steering == true) {
    esc__direction_processor(3);
  } else {
    dbc_MOTOR_STEERING_s *decoded_steering_value_from_driver = can_driver__get_driver_steering();
    servo__steer_processor(decoded_steering_value_from_driver->MOTOR_STEERING_direction);
    if (callback_count % 10 == 0)
      rpm__calculate_speed_kph();

    decoded_speed_value_from_driver = can_driver__get_driver_required_motor_speed();

    switch ((int16_t)decoded_speed_value_from_driver.MOTOR_SPEED_processed) {
    case 0 ... 2: {
      current_motor_state = MOTOR_REVERSE;
      break;
    }
    case 3: {
      current_motor_state = MOTOR_NEUTRAL;
      break;
    }
    case 4 ... 6: {
      current_motor_state = MOTOR_FORWARD;
      break;
    }
    default: { current_motor_state = MOTOR_UNDEFINED_STATE; }
    };

    motor__run_state_machine_10hz(current_motor_state, callback_count);
    last_state = current_motor_state;
#if DEBUG_STATE_MACHINE == 1
    printf("\nLast state = %d", last_state);
    printf("\nDriver value: %lf", (double)decoded_steering_value_from_driver.MOTOR_STEERING_direction);
#endif
  }
}

/**
 * STATIC FUNCTION DEFINITIONS
 */
static void motor__run_state_machine_10hz(current_motor_state_e curr_motor_state, uint32_t callback_count) {
  if (curr_motor_state != last_state && last_state != MOTOR_INIT) {
    state_change = true;
  }
  switch (curr_motor_state) {

  case MOTOR_REVERSE:
    if (state_change == false) {
      esc__direction_processor(decoded_speed_value_from_driver.MOTOR_SPEED_processed);
#if DEBUG_STATE_MACHINE == 1
      printf("\nMOTOR_REVERSE if case entered");
#endif
    } else {
#if DEBUG_STATE_MACHINE == 1
      printf("\nMOTOR_REVERSE else case entered");
#endif
      if (curr_motor_state != last_state && state_change == true) {
        entry_time_of_state_change = callback_count;
      }
      if (callback_count == entry_time_of_state_change + 1) {
        esc__direction_processor(decoded_speed_value_from_driver.MOTOR_SPEED_processed);
      }
      if ((callback_count > (entry_time_of_state_change + 1)) && callback_count <= (entry_time_of_state_change + 30)) {
        esc__direction_processor(3);
      }
      if (callback_count > (entry_time_of_state_change + 30)) {
        state_change = false;
        printf("\nCase 0...2 Transition Complete %f", decoded_speed_value_from_driver.MOTOR_SPEED_processed);
        esc__direction_processor(decoded_speed_value_from_driver.MOTOR_SPEED_processed);
      }
    }
    break;
  case MOTOR_NEUTRAL:
    esc__direction_processor(decoded_speed_value_from_driver.MOTOR_SPEED_processed);
    state_change = false;
    break;

  case MOTOR_FORWARD:
    if (state_change == false) {
      esc__direction_processor(decoded_speed_value_from_driver.MOTOR_SPEED_processed);
#if DEBUG_STATE_MACHINE == 1
      printf("\nMOTOR_FORWARD if case entered");
#endif
    } else {
#if DEBUG_STATE_MACHINE == 1
      printf("\nMOTOR_FORWARD else case entered");
#endif
      if (curr_motor_state != last_state && state_change == true) {
        entry_time_of_state_change = callback_count;
      }
      if (callback_count == entry_time_of_state_change + 1) {
        esc__direction_processor(decoded_speed_value_from_driver.MOTOR_SPEED_processed);
      }
      if ((callback_count > (entry_time_of_state_change + 1)) && callback_count <= (entry_time_of_state_change + 30)) {
        esc__direction_processor(3);
      }
      if (callback_count > (entry_time_of_state_change + 30)) {
        state_change = false;
        printf("\nCase 4...6 Transition Complete: %f", decoded_speed_value_from_driver.MOTOR_SPEED_processed);
        esc__direction_processor(decoded_speed_value_from_driver.MOTOR_SPEED_processed);
      }
    }
    break;

  default:
    printf("\nIncorrect value: MOTOR_UNDEFINED_STATE");
  }
}