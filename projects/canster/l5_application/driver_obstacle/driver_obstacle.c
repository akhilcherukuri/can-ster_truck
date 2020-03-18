#include "driver_obstacle.h"
#include <stdint.h>

/**
 * STATIC VARIABLES
 */
static const uint8_t DISTANCE_THRESHOLD = 20; // cms
static dbc_SENSOR_SONARS_s sensor_sonar;

/**
 * STATIC FUNCTION DECLARATIONS
 */
static int16_t driver_obstacle__obstacle_detected();

/**
 * FUNCTIONS
 */
void driver_obstacle__process_input(dbc_SENSOR_SONARS_s *sonar) {
  sensor_sonar = *sonar; // copy
}

// Generate the motor commands here
// TODO, Return a value out
dbc_MOTOR_STEERING_s driver_obstacle__get_motor_commands() {
  // 0 -> straight
  dbc_MOTOR_STEERING_s motor_steering = {{0}, 0};

  if (sensor_sonar.SENSOR_SONARS_middle <= DISTANCE_THRESHOLD) {
    // do something here
    motor_steering.MOTOR_STEERING_direction = driver_obstacle__obstacle_detected();
  }

  return motor_steering;
}

/**
 * STATIC FUNCTION DECLARATIONS
 */
// TODO, Improve this logic later
// TODO (IMPORTANT), Add Reverse value
// TODO (optional), Add -1/1 -> 45 degrees
static int16_t driver_obstacle__obstacle_detected() {

  int16_t return_value = 0;

  if (sensor_sonar.SENSOR_SONARS_left > sensor_sonar.SENSOR_SONARS_right &&
      sensor_sonar.SENSOR_SONARS_left > DISTANCE_THRESHOLD) {
    // Move left
    return_value = -2;
  } else if (sensor_sonar.SENSOR_SONARS_right > sensor_sonar.SENSOR_SONARS_left &&
             sensor_sonar.SENSOR_SONARS_right > DISTANCE_THRESHOLD) {
    // Move right
    return_value = +2;
  } else {
    // edge case, REVERSE
  }

  return return_value;
}