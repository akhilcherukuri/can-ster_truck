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

void test_destination_reached_or_not(void) {
  key.MOTOR_KEY_val = 1;
  is_destination_reached = true;
  can_sensor__get_motor_key_ExpectAndReturn(key);
  can_sensor__get_sensor_lidar_ExpectAndReturn(sensor_lidar);
  driver_obstacle__get_motor_commands(0);
  TEST_ASSERT_EQUAL(motor_speed_value, MOTOR_SPEED_neutral);
}

void test_driver_obstacle__sensor_lidar_middle_greater_than_150() {
  dbc_MOTOR_STEERING_s steering;
  dbc_SENSOR_LIDAR_s sensor_lidar_test = {{0}, 200, 200, 200, 200};
  dbc_SENSOR_LIDAR_s sensor_lidar_test1 = {{0}, 200, 99, 101, 200};
  dbc_SENSOR_LIDAR_s sensor_lidar_test2 = {{0}, 200, 100, 50, 200};
  dbc_SENSOR_LIDAR_s sensor_lidar_test3 = {{0}, 200, 50, 50, 200};

  key.MOTOR_KEY_val = 1;
  is_destination_reached = false;
  can_sensor__get_motor_key_ExpectAndReturn(key);
  can_sensor__get_sensor_lidar_ExpectAndReturn(sensor_lidar_test);
  driver_obstacle__get_motor_commands(0);
  TEST_ASSERT_EQUAL_INT(motor_speed_value, MOTOR_SPEED_forward_medium);

  can_sensor__get_motor_key_ExpectAndReturn(key);
  can_sensor__get_sensor_lidar_ExpectAndReturn(sensor_lidar_test1);
  steering = driver_obstacle__get_motor_commands(0);
  TEST_ASSERT_EQUAL_INT(steering.MOTOR_STEERING_direction, MOTOR_STEERING_slight_right);

  can_sensor__get_motor_key_ExpectAndReturn(key);
  can_sensor__get_sensor_lidar_ExpectAndReturn(sensor_lidar_test2);
  steering = driver_obstacle__get_motor_commands(0);
  TEST_ASSERT_EQUAL_INT(steering.MOTOR_STEERING_direction, MOTOR_STEERING_slight_left);

  can_sensor__get_motor_key_ExpectAndReturn(key);
  can_sensor__get_sensor_lidar_ExpectAndReturn(sensor_lidar_test3);
  steering = driver_obstacle__get_motor_commands(0);
  TEST_ASSERT_EQUAL_INT(steering.MOTOR_STEERING_direction, MOTOR_STEERING_straight);
}

void test_driver_obstacle__sensor_lidar_middle_less_than_150() {
  dbc_MOTOR_STEERING_s steering;
  dbc_SENSOR_LIDAR_s sensor_lidar_test = {{0}, 121, 50, 51, 200};
  dbc_SENSOR_LIDAR_s sensor_lidar_test1 = {{0}, 121, 102, 101, 200};
  dbc_SENSOR_LIDAR_s sensor_lidar_test2 = {{0}, 121, 101, 102, 200};
  dbc_SENSOR_LIDAR_s sensor_lidar_test3 = {{0}, 98, 50, 50, 200};
  dbc_SENSOR_LIDAR_s sensor_lidar_test4 = {{0}, 121, 101, 98, 200};
  dbc_SENSOR_LIDAR_s sensor_lidar_test5 = {{0}, 121, 98, 101, 200};
  // float MOTOR_SPEED_reverse_slow = 0, MOTOR_STEERING_hard_left = 3.3, MOTOR_STEERING_hard_right = 4.4;
  key.MOTOR_KEY_val = 1;
  is_destination_reached = false;
  can_sensor__get_motor_key_ExpectAndReturn(key);
  can_sensor__get_sensor_lidar_ExpectAndReturn(sensor_lidar_test);
  sensor_sonar.SENSOR_SONARS_middle = 75;
  driver_obstacle__get_motor_commands(0);
  TEST_ASSERT_EQUAL_FLOAT(motor_speed_value, MOTOR_SPEED_reverse_slow);

  TEST_ASSERT_FALSE(motor_state_fwd);

  can_sensor__get_motor_key_ExpectAndReturn(key);
  can_sensor__get_sensor_lidar_ExpectAndReturn(sensor_lidar_test1);
  steering = driver_obstacle__get_motor_commands(0);
  TEST_ASSERT_EQUAL_INT(steering.MOTOR_STEERING_direction, MOTOR_STEERING_hard_left);

  can_sensor__get_motor_key_ExpectAndReturn(key);
  can_sensor__get_sensor_lidar_ExpectAndReturn(sensor_lidar_test2);
  steering = driver_obstacle__get_motor_commands(0);
  TEST_ASSERT_EQUAL_INT(steering.MOTOR_STEERING_direction, MOTOR_STEERING_hard_right);

  can_sensor__get_motor_key_ExpectAndReturn(key);
  can_sensor__get_sensor_lidar_ExpectAndReturn(sensor_lidar_test4);
  steering = driver_obstacle__get_motor_commands(0);
  TEST_ASSERT_EQUAL_INT(steering.MOTOR_STEERING_direction, MOTOR_STEERING_hard_left);

  can_sensor__get_motor_key_ExpectAndReturn(key);
  can_sensor__get_sensor_lidar_ExpectAndReturn(sensor_lidar_test5);
  steering = driver_obstacle__get_motor_commands(0);
  TEST_ASSERT_EQUAL_INT(steering.MOTOR_STEERING_direction, MOTOR_STEERING_hard_right);

  can_sensor__get_motor_key_ExpectAndReturn(key);
  can_sensor__get_sensor_lidar_ExpectAndReturn(sensor_lidar_test3);
  driver_obstacle__get_motor_commands(0);
  TEST_ASSERT_EQUAL_FLOAT(motor_speed_value, MOTOR_SPEED_reverse_slow);
  TEST_ASSERT_FALSE(motor_state_fwd);
}

void test_neutral_check(void) {
  motor_state_fwd = false;
  key.MOTOR_KEY_val = 1;
  sensor_sonar.SENSOR_SONARS_middle = 69.0;
  sensor_lidar.SENSOR_LIDAR_back = 99;
  can_sensor__get_motor_key_ExpectAndReturn(key);
  can_sensor__get_sensor_lidar_ExpectAndReturn(sensor_lidar);
  driver_obstacle__get_motor_commands(0);
  TEST_ASSERT_EQUAL_FLOAT(motor_speed_value, MOTOR_SPEED_neutral);

  sensor_sonar.SENSOR_SONARS_middle = 72.0;
  sensor_lidar.SENSOR_LIDAR_back = 99;
  can_sensor__get_motor_key_ExpectAndReturn(key);
  can_sensor__get_sensor_lidar_ExpectAndReturn(sensor_lidar);
  driver_obstacle__get_motor_commands(0);
  TEST_ASSERT_EQUAL_FLOAT(motor_speed_value, MOTOR_SPEED_neutral);

  sensor_sonar.SENSOR_SONARS_middle = 69.0;
  sensor_lidar.SENSOR_LIDAR_back = 101;
  can_sensor__get_motor_key_ExpectAndReturn(key);
  can_sensor__get_sensor_lidar_ExpectAndReturn(sensor_lidar);
  driver_obstacle__get_motor_commands(0);
  TEST_ASSERT_EQUAL_FLOAT(motor_speed_value, MOTOR_SPEED_neutral);

  sensor_sonar.SENSOR_SONARS_middle = 71;
  sensor_lidar.SENSOR_LIDAR_back = 101;
  can_sensor__get_motor_key_ExpectAndReturn(key);
  can_sensor__get_sensor_lidar_ExpectAndReturn(sensor_lidar);
  driver_obstacle__get_motor_commands(0);
  TEST_ASSERT_NOT_EQUAL(motor_speed_value, MOTOR_SPEED_neutral);

  motor_state_fwd = true;
  can_sensor__get_motor_key_ExpectAndReturn(key);
  can_sensor__get_sensor_lidar_ExpectAndReturn(sensor_lidar);
  driver_obstacle__get_motor_commands(0);
  TEST_ASSERT_NOT_EQUAL(motor_speed_value, MOTOR_SPEED_neutral);
}
