#include "driver_obstacle.h"
#include <stdint.h>
#include <stdlib.h>

#define DRIVER_OBSTACLE_DEBUG 1

#if DRIVER_OBSTACLE_DEBUG == 1
#include <stdio.h>
#endif

/**
 * STATIC VARIABLES
 */
static const uint8_t DISTANCE_THRESHOLD = 100; // cms

static dbc_SENSOR_SONARS_s sensor_sonar;
static dbc_GEO_DEGREE_s geo_degree;

static float motor_speed_value;

/**
 * STATIC FUNCTION DECLARATIONS
 */
static int16_t driver_obstacle__tilt_left_or_right();
static int16_t driver_obstacle__obstacle_detected();

static int16_t driver_obstacle__turn_according_to_degree(float diff);
/**
 * FUNCTIONS
 */
void driver_obstacle__process_input(dbc_SENSOR_SONARS_s *sonar) {
  sensor_sonar = *sonar; // copy
}

void driver_obstacle__set_geo_controller_direction(dbc_GEO_DEGREE_s *degree) {
  geo_degree = *degree; // copy
}

float driver_obstacle__get_motor_speed_value() { return motor_speed_value; }

// Generate the motor commands here
// TODO, Return a value out
dbc_MOTOR_STEERING_s driver_obstacle__get_motor_commands() {
  // 0 -> straight
  dbc_MOTOR_STEERING_s motor_steering = {{0}, 0};
  motor_speed_value = 5; // fwd med
  if (sensor_sonar.SENSOR_SONARS_middle > DISTANCE_THRESHOLD) {
    // Position yourself to the GPS Coordinates and move towards the target
    motor_steering.MOTOR_STEERING_direction = driver_obstacle__move_to_destination();
    #if DRIVER_OBSTACLE_DEBUG == 1
    printf("\nSteering value computed by Geo logic: %d", motor_steering.MOTOR_STEERING_direction);
    #endif
  } else {
    /* To check if turning radius is healthy and large enough */
    if (sensor_sonar.SENSOR_SONARS_middle < (DISTANCE_THRESHOLD / 2.0)) {
      motor_speed_value = 3; // neutral = stop
    } else {
      if (sensor_sonar.SENSOR_SONARS_left < DISTANCE_THRESHOLD &&
          sensor_sonar.SENSOR_SONARS_right < DISTANCE_THRESHOLD) {
        motor_speed_value = 3; // neutral = stop
      } else {
        if (sensor_sonar.SENSOR_SONARS_left > sensor_sonar.SENSOR_SONARS_right) {
          motor_steering.MOTOR_STEERING_direction = -1; // driver_obstacle__obstacle_detected();
        } else if (sensor_sonar.SENSOR_SONARS_left <= sensor_sonar.SENSOR_SONARS_right) {
          motor_steering.MOTOR_STEERING_direction = 1;
        }
      }
    }
    #if DRIVER_OBSTACLE_DEBUG == 1
    printf("\nSteering value computed due to Ultrasonic sensor obstacles: %d", motor_steering.MOTOR_STEERING_direction);
    #endif
  }

  return motor_steering;
}

/**
 * STATIC FUNCTION DECLARATIONS
 */

/**
 * NOTE: + means right, - means left
 *
 * Explanation: Say our current degree is 90
 * Our possible points chosen can be 40 (right), 150 (left)
 * 300 (right) and 220 (left)
 *
 * Math used
 *
 * Example 1:
 * current - required i.e 90 - 40 = 50 (right)
 * However another path could be (50 - 360) => -310 (left)
 * We chose 50 right since it is easier to turn
 *
 * Example 2:
 * 90 - 150 = -60 (left)
 * Another path could be used (-60 + 360) => 300 (right)
 * We chose -60 left since it is easier to turn
 *
 * Example 3:
 * 90 - 300 = -210 (left)
 * Another path = -210 + 360 => 150 (right)
 * We chose 150 right (easier to turn)
 *
 * Example 4:
 * 270 - 220 = 50 (right)
 * 270 - 300 = 30 (left)
 *
 * 270 - 40 = 230, -130
 *
 */
int16_t driver_obstacle__move_to_destination() {
  // DONE, Write this logic
  float diff = geo_degree.GEO_DEGREE_current - geo_degree.GEO_DEGREE_required;
  if (abs(diff) > 180) {
    if (diff < 0) {
      diff = diff + 360;
    } else if (diff > 0) {
      diff = diff - 360;
    } else {
      // do nothing
    }
  }

  return driver_obstacle__turn_according_to_degree(diff);
}

// TODO, Stop Condition
static int16_t driver_obstacle__turn_according_to_degree(float diff) {
  int16_t rval = 0;

  if (diff > 0) {
    // Go little right
    rval = 1;
  } else if (diff < 0) {
    // little left
    rval = -1;
  } else {
    // We have reached our destination (STOP Condition)
  }

  return rval;
}

static int16_t driver_obstacle__tilt_left_or_right() {
  int16_t return_value = 0;

  if (sensor_sonar.SENSOR_SONARS_left <= DISTANCE_THRESHOLD && sensor_sonar.SENSOR_SONARS_right > DISTANCE_THRESHOLD) {
    // Move a little right
    return_value = +1;

  } else if (sensor_sonar.SENSOR_SONARS_right <= DISTANCE_THRESHOLD &&
             sensor_sonar.SENSOR_SONARS_left > DISTANCE_THRESHOLD) {
    // Move a little left
    return_value = -1;

  } else {
    // TODO, Reverse Condition
    // both right and left are <= DISTANCE THRESHOLD
    // Reverse condition
  }
  return return_value;
}

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