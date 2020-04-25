#include "unity.h"

#include "driver_obstacle.c"

void setUp() {
  sensor_sonar.SENSOR_SONARS_left = 0;
  sensor_sonar.SENSOR_SONARS_middle = 0;
  sensor_sonar.SENSOR_SONARS_right = 0;

  geo_degree.GEO_DEGREE_current = 0;
  geo_degree.GEO_DEGREE_required = 0;
}
void tearDown() {}

void test_driver_obstacle__obstacle_detected_left() {
  sensor_sonar.SENSOR_SONARS_left = 50;
  sensor_sonar.SENSOR_SONARS_right = 30;

  int16_t isleft = driver_obstacle__obstacle_detected();
  TEST_ASSERT_EQUAL_INT16(isleft, DRIVER_STEERING_direction_LEFT);
}

void test_driver_obstacle__obstacle_detected_right() {
  sensor_sonar.SENSOR_SONARS_left = 30;
  sensor_sonar.SENSOR_SONARS_right = 50;

  int16_t isright = driver_obstacle__obstacle_detected();
  TEST_ASSERT_EQUAL_INT16(isright, DRIVER_STEERING_direction_RIGHT);
}

void test_driver_obstacle__tilt_left(void) {
  sensor_sonar.SENSOR_SONARS_right = DISTANCE_THRESHOLD - 10;
  sensor_sonar.SENSOR_SONARS_left = DISTANCE_THRESHOLD + 10;

  int16_t left = driver_obstacle__tilt_left_or_right();
  TEST_ASSERT_EQUAL_INT16(left, DRIVER_STEERING_direction_SLIGHT_LEFT);
}

void test_driver_obstacle__tilt_right(void) {
  sensor_sonar.SENSOR_SONARS_right = DISTANCE_THRESHOLD + 10;
  sensor_sonar.SENSOR_SONARS_left = DISTANCE_THRESHOLD - 10;

  int16_t right = driver_obstacle__tilt_left_or_right();
  TEST_ASSERT_EQUAL_INT16(right, DRIVER_STEERING_direction_SLIGHT_RIGHT);
}

void test_driver_obstacle__turn_according_to_degree_left(void) {
  int16_t turn = driver_obstacle__turn_according_to_degree(-20);
  TEST_ASSERT_EQUAL_INT16(turn, DRIVER_STEERING_direction_SLIGHT_LEFT);
}

void test_driver_obstacle__turn_according_to_degree_right(void) {
  int16_t turn = driver_obstacle__turn_according_to_degree(+50);
  TEST_ASSERT_EQUAL_INT16(turn, DRIVER_STEERING_direction_SLIGHT_RIGHT);
}

void test_driver_obstacle__turn_according_to_degree_straight(void) {
  int16_t turn = driver_obstacle__turn_according_to_degree(0);
  TEST_ASSERT_EQUAL_INT16(turn, DRIVER_STEERING_direction_STRAIGHT);
}

void test_driver_obstacle__move_to_destination_top_quadrant() {
  int16_t movement;
  geo_degree.GEO_DEGREE_current = 90;

  // P1 -> First Quadrant
  geo_degree.GEO_DEGREE_required = 45;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, DRIVER_STEERING_direction_SLIGHT_RIGHT);

  // P2 ->
  geo_degree.GEO_DEGREE_required = 90;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, DRIVER_STEERING_direction_STRAIGHT);

  // P3 -> Second Quadrant
  geo_degree.GEO_DEGREE_required = 135;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, DRIVER_STEERING_direction_SLIGHT_LEFT);

  // P4 -> Third Quadrant
  geo_degree.GEO_DEGREE_required = 180 + 45;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, DRIVER_STEERING_direction_SLIGHT_LEFT);

  // P5 -> Fourth Quadrant
  geo_degree.GEO_DEGREE_required = 180 + 45 + 90;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, DRIVER_STEERING_direction_SLIGHT_RIGHT);
}

void test_driver_obstacle__move_to_destination_bottom_quadrant() {
  int16_t movement;
  geo_degree.GEO_DEGREE_current = 270;

  // P1 -> First Quadrant
  geo_degree.GEO_DEGREE_required = 45;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, DRIVER_STEERING_direction_SLIGHT_LEFT);

  // P2 -> Second Quadrant
  geo_degree.GEO_DEGREE_required = 135;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, DRIVER_STEERING_direction_SLIGHT_RIGHT);

  // P3 -> Third Quadrant
  geo_degree.GEO_DEGREE_required = 180 + 45;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, DRIVER_STEERING_direction_SLIGHT_RIGHT);

  // P4 -> Equal Quadrant
  geo_degree.GEO_DEGREE_required = 270;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, DRIVER_STEERING_direction_STRAIGHT);

  // P5 -> Fourth Quadrant
  geo_degree.GEO_DEGREE_required = 180 + 45 + 90;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, DRIVER_STEERING_direction_SLIGHT_LEFT);
}

void test_driver_obstacle__get_motor_commands_left_right() {
  sensor_sonar.SENSOR_SONARS_middle = (DISTANCE_THRESHOLD)-5;
  sensor_sonar.SENSOR_SONARS_left = 50;
  sensor_sonar.SENSOR_SONARS_right = 30;

  dbc_DRIVER_STEERING_s steering = driver_obstacle__get_motor_commands();
  TEST_ASSERT_EQUAL(steering.DRIVER_STEERING_direction, DRIVER_STEERING_direction_LEFT);

  sensor_sonar.SENSOR_SONARS_left = 30;
  sensor_sonar.SENSOR_SONARS_right = 50;
  steering = driver_obstacle__get_motor_commands();
  TEST_ASSERT_EQUAL(steering.DRIVER_STEERING_direction, DRIVER_STEERING_direction_RIGHT);
}
