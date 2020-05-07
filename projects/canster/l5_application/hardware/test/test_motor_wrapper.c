#include "unity.h"

#include "Mockcan_driver_node.h"
#include "Mockesc.h"
#include "Mockrpm.h"
#include "Mockservo.h"

#include "motor_wrapper.h"

// #include "motor_wrapper.c"
#include "project.h"

void setUp() {}
void tearDown() {}

void test_motor__init() {
  servo__steer_init_Expect();
  esc__init_Expect();
  rpm__init_Expect();
  motor__init();
}

void test_motor__run_10hz() {
  dbc_MOTOR_STEERING_s decoded_steering_value_from_driver = {{0}, MOTOR_STEERING_straight};
  dbc_MOTOR_SPEED_s decoded_speed_value_from_driver = {{0}, 3};
  get_mia_steering_ExpectAndReturn(false);
  can_driver__get_driver_steering_ExpectAndReturn(&decoded_steering_value_from_driver);
  servo__steer_processor_Expect(decoded_steering_value_from_driver.MOTOR_STEERING_direction);
  can_driver__get_driver_required_motor_speed_ExpectAndReturn(decoded_speed_value_from_driver);
  rpm__calculate_speed_kph_ExpectAndReturn(0);
  esc__direction_processor_Expect(3);
  motor__run_10hz(0);
}

void test_motor__run_10hz_forward() {
  dbc_MOTOR_STEERING_s decoded_steering_value_from_driver = {{0}, MOTOR_STEERING_straight};
  dbc_MOTOR_SPEED_s decoded_speed_value_from_driver = {{0}, 2};
  get_mia_steering_ExpectAndReturn(false);
  can_driver__get_driver_steering_ExpectAndReturn(&decoded_steering_value_from_driver);
  servo__steer_processor_Expect(decoded_steering_value_from_driver.MOTOR_STEERING_direction);
  can_driver__get_driver_required_motor_speed_ExpectAndReturn(decoded_speed_value_from_driver);
  rpm__calculate_speed_kph_ExpectAndReturn(0);

  motor__run_10hz(0); // Zeroth Callback
  get_mia_steering_ExpectAndReturn(false);
  can_driver__get_driver_steering_ExpectAndReturn(&decoded_steering_value_from_driver);
  servo__steer_processor_Expect(decoded_steering_value_from_driver.MOTOR_STEERING_direction);
  can_driver__get_driver_required_motor_speed_ExpectAndReturn(decoded_speed_value_from_driver);
  esc__direction_processor_Expect(decoded_speed_value_from_driver.MOTOR_SPEED_processed);
  motor__run_10hz(1); // First Callback
}

void test_motor__run_10hz_backward() {
  dbc_MOTOR_STEERING_s decoded_steering_value_from_driver = {{0}, MOTOR_STEERING_straight};
  dbc_MOTOR_SPEED_s decoded_speed_value_from_driver = {{0}, 2};
  get_mia_steering_ExpectAndReturn(false);
  can_driver__get_driver_steering_ExpectAndReturn(&decoded_steering_value_from_driver);
  servo__steer_processor_Expect(decoded_steering_value_from_driver.MOTOR_STEERING_direction);
  can_driver__get_driver_required_motor_speed_ExpectAndReturn(decoded_speed_value_from_driver);
  rpm__calculate_speed_kph_ExpectAndReturn(0);

  motor__run_10hz(0); // Zeroth Callback
  get_mia_steering_ExpectAndReturn(false);
  can_driver__get_driver_steering_ExpectAndReturn(&decoded_steering_value_from_driver);
  servo__steer_processor_Expect(decoded_steering_value_from_driver.MOTOR_STEERING_direction);
  can_driver__get_driver_required_motor_speed_ExpectAndReturn(decoded_speed_value_from_driver);
  esc__direction_processor_Expect(decoded_speed_value_from_driver.MOTOR_SPEED_processed);
  motor__run_10hz(1); // First Callback
}