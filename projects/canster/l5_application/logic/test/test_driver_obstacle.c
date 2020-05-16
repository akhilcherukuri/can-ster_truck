#include "unity.h"

#include "Mockcan_sensor_node.h"

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
  sensor_sonar.SENSOR_SONARS_left = 150;
  sensor_sonar.SENSOR_SONARS_right = 30;

  int16_t isleft = driver_obstacle__obstacle_detected();
  TEST_ASSERT_EQUAL_INT16(isleft, MOTOR_STEERING_hard_left);
}

void test_driver_obstacle__obstacle_detected_right() {
  sensor_sonar.SENSOR_SONARS_left = 30;
  sensor_sonar.SENSOR_SONARS_right = 150;

  int16_t isright = driver_obstacle__obstacle_detected();
  TEST_ASSERT_EQUAL_INT16(isright, MOTOR_STEERING_hard_right);
}

void test_driver_obstacle__tilt_left(void) {
  sensor_sonar.SENSOR_SONARS_right = DISTANCE_THRESHOLD_CM - 10.0;
  sensor_sonar.SENSOR_SONARS_left = DISTANCE_THRESHOLD_CM + 10.0;

  int16_t left = driver_obstacle__tilt_left_or_right();
  TEST_ASSERT_EQUAL_INT16(left, MOTOR_STEERING_slight_left);
}

void test_driver_obstacle__tilt_right(void) {
  sensor_sonar.SENSOR_SONARS_right = DISTANCE_THRESHOLD_CM + 10.0;
  sensor_sonar.SENSOR_SONARS_left = DISTANCE_THRESHOLD_CM - 10.0;

  int16_t right = driver_obstacle__tilt_left_or_right();
  TEST_ASSERT_EQUAL_INT16(right, MOTOR_STEERING_slight_right);
}

void test_driver_obstacle__turn_according_to_degree_left(void) {
  int16_t turn = driver_obstacle__turn_according_to_degree(-20);
  TEST_ASSERT_EQUAL_INT16(turn, MOTOR_STEERING_slight_left);
}

void test_driver_obstacle__turn_according_to_degree_right(void) {
  int16_t turn = driver_obstacle__turn_according_to_degree(+50);
  TEST_ASSERT_EQUAL_INT16(turn, MOTOR_STEERING_slight_right);
}

void test_driver_obstacle__turn_according_to_degree_straight(void) {
  int16_t turn = driver_obstacle__turn_according_to_degree(0);
  TEST_ASSERT_EQUAL_INT16(turn, MOTOR_STEERING_straight);
}

void test_driver_obstacle__move_to_destination_top_quadrant() {
  int16_t movement;
  geo_degree.GEO_DEGREE_current = 90;

  // P1 -> First Quadrant
  geo_degree.GEO_DEGREE_required = 45;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, MOTOR_STEERING_slight_right);

  // P2 ->
  geo_degree.GEO_DEGREE_required = 90;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, MOTOR_STEERING_straight);

  // P3 -> Second Quadrant
  geo_degree.GEO_DEGREE_required = 135;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, MOTOR_STEERING_slight_left);

  // P4 -> Third Quadrant
  geo_degree.GEO_DEGREE_required = 180 + 45;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, MOTOR_STEERING_slight_left);

  // P5 -> Fourth Quadrant
  geo_degree.GEO_DEGREE_required = 180 + 45 + 90;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, MOTOR_STEERING_slight_right);
}

void test_driver_obstacle__move_to_destination_bottom_quadrant() {
  int16_t movement;
  geo_degree.GEO_DEGREE_current = 270;

  // P1 -> First Quadrant
  geo_degree.GEO_DEGREE_required = 45;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, -1);

  // P2 -> Second Quadrant
  geo_degree.GEO_DEGREE_required = 135;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, 1);

  // P3 -> Third Quadrant
  geo_degree.GEO_DEGREE_required = 180 + 45;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, 1);

  // P4 -> Equal Quadrant
  geo_degree.GEO_DEGREE_required = 270;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, 0);

  // P5 -> Fourth Quadrant
  geo_degree.GEO_DEGREE_required = 180 + 45 + 90;

  movement = driver_obstacle__move_to_destination();
  TEST_ASSERT_EQUAL_INT16(movement, -1);
}

void test_driver_obstacle__get_motor_commands_left_right() {
  // Come back to this later
  TEST_IGNORE();

  can_sensor__get_sensor_lidar_ExpectAndReturn(sensor_lidar);

  sensor_sonar.SENSOR_SONARS_middle = (DISTANCE_THRESHOLD_CM)-5.0;
  sensor_sonar.SENSOR_SONARS_left = 150;
  sensor_sonar.SENSOR_SONARS_right = 30;

  dbc_MOTOR_STEERING_s steering = driver_obstacle__get_motor_commands(0);
  TEST_ASSERT_EQUAL(steering.MOTOR_STEERING_direction, MOTOR_STEERING_hard_left);

  sensor_sonar.SENSOR_SONARS_left = 30;
  sensor_sonar.SENSOR_SONARS_right = 150;
  steering = driver_obstacle__get_motor_commands(0);
  TEST_ASSERT_EQUAL(steering.MOTOR_STEERING_direction, MOTOR_STEERING_hard_right);
}

void test_driver_obstacle__get_motor_commands_left_right_for_lidar() {
  // TODO, Write this test
}
