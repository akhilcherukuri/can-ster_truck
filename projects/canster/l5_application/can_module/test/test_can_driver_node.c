#include "unity.h"

#include "can_mia_configurations.c"
#include "project.h"

#include "Mockboard_io.h"
#include "Mockcan_handler.h"
#include "Mockgpio.h"

#include "can_driver_node.c"
#include "who_am_i.h"

void setUp() {
  driver_steering.mia_info.mia_counter = 0;
  driver_required_motor_speed.mia_info.mia_counter = 0;
  driver_heartbeat.mia_info.mia_counter = 0;
}
void tearDown() {}

void test_can_driver__driver_heartbeat_mia_true() {
  gpio_s gpio;
  board_io__get_led2_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);

  can_driver__driver_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(driver_heartbeat.mia_info.mia_counter, 1000);

  can_driver__driver_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(driver_heartbeat.mia_info.mia_counter, 2000);

  can_driver__driver_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(driver_heartbeat.mia_info.mia_counter, 3000);
}

void test_can_driver__driver_heartbeat_mia_false() {
  can_driver__driver_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(driver_heartbeat.mia_info.mia_counter, 1000);

  can_driver__driver_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(driver_heartbeat.mia_info.mia_counter, 2000);

  driver_heartbeat.mia_info.mia_counter = 0;

  can_driver__driver_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(driver_heartbeat.mia_info.mia_counter, 1000);
}

void test_can_driver__motor_speed_mia_true() {
  gpio_s gpio;
  board_io__get_led2_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);

  can_driver__motor_speed_mia();
  TEST_ASSERT_EQUAL_UINT32(driver_required_motor_speed.mia_info.mia_counter, 1000);

  can_driver__motor_speed_mia();
  TEST_ASSERT_EQUAL_UINT32(driver_required_motor_speed.mia_info.mia_counter, 2000);

  can_driver__motor_speed_mia();
  TEST_ASSERT_EQUAL_UINT32(driver_required_motor_speed.mia_info.mia_counter, 3000);
}

void test_can_driver__motor_speed_mia_false() {
  can_driver__motor_speed_mia();
  TEST_ASSERT_EQUAL_UINT32(driver_required_motor_speed.mia_info.mia_counter, 1000);

  can_driver__motor_speed_mia();
  TEST_ASSERT_EQUAL_UINT32(driver_required_motor_speed.mia_info.mia_counter, 2000);

  driver_required_motor_speed.mia_info.mia_counter = 0;

  can_driver__motor_speed_mia();
  TEST_ASSERT_EQUAL_UINT32(driver_required_motor_speed.mia_info.mia_counter, 1000);
}

void test_can_driver__motor_steering_mia_true() {
  gpio_s gpio;
  board_io__get_led2_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);

  can_driver__motor_steering_mia();
  TEST_ASSERT_EQUAL_UINT32(driver_steering.mia_info.mia_counter, 1000);

  can_driver__motor_steering_mia();
  TEST_ASSERT_EQUAL_UINT32(driver_steering.mia_info.mia_counter, 2000);

  can_driver__motor_steering_mia();
  TEST_ASSERT_EQUAL_UINT32(driver_steering.mia_info.mia_counter, 3000);
}

void test_can_driver__motor_steering_mia_false() {
  can_driver__motor_steering_mia();
  TEST_ASSERT_EQUAL_UINT32(driver_steering.mia_info.mia_counter, 1000);

  can_driver__motor_steering_mia();
  TEST_ASSERT_EQUAL_UINT32(driver_steering.mia_info.mia_counter, 2000);

  driver_steering.mia_info.mia_counter = 0;

  can_driver__motor_steering_mia();
  TEST_ASSERT_EQUAL_UINT32(driver_steering.mia_info.mia_counter, 1000);
}