#include "can_driver_logic.h"

// Assuming this data structure as output from the DBC
static dbc_MOTOR_STEERING_s steering_val;

/* TODO: Integrate actual RPM Sensor driver API here in order to fetch the *
 * actual wheel speed. For now assuming RPM gives 60.0kph (hardcoded).     */
static dbc_MOTOR_SPEED_FEEDBACK_s current_wheel_speed_val = {{0}, 60.0};

static dbc_MOTOR_SPEED_FEEDBACK_s current_wheel_speed_local = {};

static dbc_SENSOR_SONARS_s sensor_sonar_local;
static dbc_SENSOR_SONARS_s sensor_threshold_values = {{0}, 300, 300, 300};
static dbc_MOTOR_SPEED_s wheel_speed_threshold_values = {{0}, 50.0};

void driver__set_sensor_input(dbc_SENSOR_SONARS_s sensor_val_from_ultrasonic_sensor) {
  sensor_sonar_local.SENSOR_SONARS_left = sensor_val_from_ultrasonic_sensor.SENSOR_SONARS_left;
  sensor_sonar_local.SENSOR_SONARS_right = sensor_val_from_ultrasonic_sensor.SENSOR_SONARS_right;
  sensor_sonar_local.SENSOR_SONARS_middle = sensor_val_from_ultrasonic_sensor.SENSOR_SONARS_middle;
}

/* Driver will decode the current wheel speed and set it locally */
void driver__set_current_wheel_speed(dbc_MOTOR_SPEED_FEEDBACK_s motor_wheel_speed_current_val_from_rpm_sensor) {
  current_wheel_speed_local.MOTOR_SPEED_current = motor_wheel_speed_current_val_from_rpm_sensor.MOTOR_SPEED_current;
}

/* Feedback Monitoring: Keep checking wheel speed from *
 * MOTOR Node and handle the speed in two scenarios.   *
 * ENCODE dbc_MOTOR_SPEED_s from DRIVER to MOTOR  */
dbc_MOTOR_SPEED_s driver__process_current_wheel_speed() {
  dbc_MOTOR_SPEED_s final_wheel_speed_sent_to_motor = {{0}, 0.0};

  /* When car travels at a faster speed (>= 50 kph) we encode and send *
   * a hardcoded lower safe value (40kph for example) to motor node,   *
   * which in turn will be used to regulate voltage fed to ESC.        */
  if (current_wheel_speed_local.MOTOR_SPEED_current >= wheel_speed_threshold_values.MOTOR_SPEED_processed)
    final_wheel_speed_sent_to_motor.MOTOR_SPEED_processed = 40.0;

  /* Stop the car when you sense obstacle very close.  *
   * Assuming 100 inches as very close (example value) */
  if (sensor_sonar_local.SENSOR_SONARS_left <= 100 || sensor_sonar_local.SENSOR_SONARS_right <= 100 ||
      sensor_sonar_local.SENSOR_SONARS_middle <= 100)
    final_wheel_speed_sent_to_motor.MOTOR_SPEED_processed = 0.0;

  // TODO Also reduce the speed whenever the car is steering

  return final_wheel_speed_sent_to_motor;
}

void driver__process_sensor_input(dbc_SENSOR_SONARS_s sensor_val) {
  sensor_sonar_local.SENSOR_SONARS_left = sensor_val.SENSOR_SONARS_left;
  sensor_sonar_local.SENSOR_SONARS_middle = sensor_val.SENSOR_SONARS_middle;
  sensor_sonar_local.SENSOR_SONARS_right = sensor_val.SENSOR_SONARS_right;
}

// 2 car is going to be steered right
// 0 indicates continue steering in same direction (straight)
// -2 car is going to be steered left
// For now, we will discard middle sensor values and assume that obstacles only appear left or right

dbc_MOTOR_STEERING_s driver__process_motor_steering(void) {
  fprintf(stderr,
          "\nSensor values from SENSOR Node used for calculating final steering:\r\nLeft = %d\r\nRight = "
          "%d\r\nMiddle = %d\r\n",
          sensor_sonar_local.SENSOR_SONARS_left, sensor_sonar_local.SENSOR_SONARS_right,
          sensor_sonar_local.SENSOR_SONARS_middle);
  if ((sensor_sonar_local.SENSOR_SONARS_left < sensor_threshold_values.SENSOR_SONARS_left) &&
      (sensor_sonar_local.SENSOR_SONARS_right < sensor_threshold_values.SENSOR_SONARS_right)) {
    if (sensor_sonar_local.SENSOR_SONARS_right > sensor_sonar_local.SENSOR_SONARS_left) {
      steering_val.MOTOR_STEERING_direction = 2;
    } else if (sensor_sonar_local.SENSOR_SONARS_left > sensor_sonar_local.SENSOR_SONARS_right) {
      steering_val.MOTOR_STEERING_direction = -2;
    } else {
      steering_val.MOTOR_STEERING_direction = 0;
    }
  } else if (sensor_sonar_local.SENSOR_SONARS_left < sensor_threshold_values.SENSOR_SONARS_left) {
    steering_val.MOTOR_STEERING_direction = 2; // move to right
  } else if (sensor_sonar_local.SENSOR_SONARS_right < sensor_threshold_values.SENSOR_SONARS_right) {
    steering_val.MOTOR_STEERING_direction = -2; // move to left
  } else {
    steering_val.MOTOR_STEERING_direction = 0; // keep steering in same/straight direction
  }
  fprintf(stderr, "\nFinal steering value = %d", steering_val.MOTOR_STEERING_direction);
  return steering_val;
}

/* Motor Node reads current wheel speed via RPM Sensor.                        *
 * So obtain the current wheen speed value and encode and send to Driver Node  */
dbc_MOTOR_SPEED_FEEDBACK_s driver__get_current_motor_wheel_speed_from_rpm_sensor(void) {
  return current_wheel_speed_val;
}