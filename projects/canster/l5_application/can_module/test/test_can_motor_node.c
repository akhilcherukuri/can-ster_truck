#include "unity.h"

#include "can_mia_configurations.c"
#include "project.h"

#include "Mockboard_io.h"
#include "Mockcan_handler.h"
#include "Mockgpio.h"

#include "can_motor_node.c"
#include "who_am_i.h"

void setUp() {
  motor_heartbeat.mia_info.mia_counter = 0;
  motor_info.mia_info.mia_counter = 0;
}

void tearDown() {}

void test_can_motor__motor_heartbeat_mia_true() {
  gpio_s gpio;
  board_io__get_led1_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);

  can_motor__motor_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(motor_heartbeat.mia_info.mia_counter, 1000);

  can_motor__motor_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(motor_heartbeat.mia_info.mia_counter, 2000);

  can_motor__motor_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(motor_heartbeat.mia_info.mia_counter, 3000);
}

void test_can_motor__motor_heartbeat_mia_false() {
  can_motor__motor_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(motor_heartbeat.mia_info.mia_counter, 1000);

  motor_heartbeat.mia_info.mia_counter = 0;

  can_motor__motor_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(motor_heartbeat.mia_info.mia_counter, 1000);
}

void test_can_motor__motor_info_mia_true() {
  // gpio_s gpio;
  // board_io__get_led1_ExpectAndReturn(gpio);
  // gpio__set_Expect(gpio);

  can_motor__motor_info_mia();
  TEST_ASSERT_EQUAL_UINT32(motor_info.mia_info.mia_counter, 1000);

  can_motor__motor_info_mia();
  TEST_ASSERT_EQUAL_UINT32(motor_info.mia_info.mia_counter, 2000);

  can_motor__motor_info_mia();
  TEST_ASSERT_EQUAL_UINT32(motor_info.mia_info.mia_counter, 3000);
}

void test_can_motor__motor_info_mia_false() {
  can_motor__motor_info_mia();
  TEST_ASSERT_EQUAL_UINT32(motor_info.mia_info.mia_counter, 1000);

  motor_info.mia_info.mia_counter = 0;

  can_motor__motor_info_mia();
  TEST_ASSERT_EQUAL_UINT32(motor_info.mia_info.mia_counter, 1000);
}
