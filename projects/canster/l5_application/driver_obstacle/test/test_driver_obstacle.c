#include "unity.h"

#include "driver_obstacle.c"

void setUp() {
  sensor_sonar.SENSOR_SONARS_left = 0;
  sensor_sonar.SENSOR_SONARS_middle = 0;
  sensor_sonar.SENSOR_SONARS_right = 0;
}
void tearDown() {}

void test_driver_obstacle__obstacle_detected_left() {
  sensor_sonar.SENSOR_SONARS_left = 50;
  sensor_sonar.SENSOR_SONARS_right = 30;

  int16_t isleft = driver_obstacle__obstacle_detected();
  TEST_ASSERT_EQUAL_INT16(isleft, -2);
}

void test_driver_obstacle__obstacle_detected_right() {
  sensor_sonar.SENSOR_SONARS_left = 30;
  sensor_sonar.SENSOR_SONARS_right = 50;

  int16_t isright = driver_obstacle__obstacle_detected();
  TEST_ASSERT_EQUAL_INT16(isright, 2);
}

void test_driver_obstacle__get_motor_commands_straight() {
  dbc_MOTOR_STEERING_s steering = driver_obstacle__get_motor_commands();
  TEST_ASSERT_EQUAL(steering.MOTOR_STEERING_direction, 0);
}

void test_driver_obstacle__get_motor_commands_left_right() {
  sensor_sonar.SENSOR_SONARS_middle = (DISTANCE_THRESHOLD)-5;
  sensor_sonar.SENSOR_SONARS_left = 50;
  sensor_sonar.SENSOR_SONARS_right = 30;

  dbc_MOTOR_STEERING_s steering = driver_obstacle__get_motor_commands();
  TEST_ASSERT_EQUAL(steering.MOTOR_STEERING_direction, -2);

  sensor_sonar.SENSOR_SONARS_left = 30;
  sensor_sonar.SENSOR_SONARS_right = 50;
  steering = driver_obstacle__get_motor_commands();
  TEST_ASSERT_EQUAL(steering.MOTOR_STEERING_direction, 2);
}
