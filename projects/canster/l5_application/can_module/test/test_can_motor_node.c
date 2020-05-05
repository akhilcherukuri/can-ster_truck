#include "unity.h"

#include "can_mia_configurations.c"
#include "project.h"

#include "Mockboard_io.h"
#include "Mockcan_handler.h"
#include "Mockgpio.h"

#include "Mocklcd_ui.h"

#include "can_motor_node.c"
#include "who_am_i.h"

void setUp() {
  motor_heartbeat.mia_info.mia_counter = 0;
  motor_wheel_speed_current_val.mia_info.mia_counter = 0;
}

void tearDown() {}

#if BOARD_MOTOR_NODE == 1
void test_can_motor__motor_heartbeat_mia_true() {}
void test_can_motor__motor_heartbeat_mia_false() {}
void test_can_motor__motor_speed_feedback_mia_true() {}
void test_can_motor__motor_speed_feedback_mia_false() {}
#else

void test_can_motor__motor_heartbeat_mia_true() {

  set_lcd_mia_led_Expect(MOTOR_LED_MIA, false);

  gpio_s gpio;
  board_io__get_led2_ExpectAndReturn(gpio);
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

void test_can_motor__motor_speed_feedback_mia_true() {
  // gpio_s gpio;
  // board_io__get_led1_ExpectAndReturn(gpio);
  // gpio__set_Expect(gpio);

  can_motor__motor_speed_feedback_mia();
  TEST_ASSERT_EQUAL_UINT32(motor_wheel_speed_current_val.mia_info.mia_counter, 1000);

  can_motor__motor_speed_feedback_mia();
  TEST_ASSERT_EQUAL_UINT32(motor_wheel_speed_current_val.mia_info.mia_counter, 2000);

  can_motor__motor_speed_feedback_mia();
  TEST_ASSERT_EQUAL_UINT32(motor_wheel_speed_current_val.mia_info.mia_counter, 3000);
}

void test_can_motor__motor_speed_feedback_mia_false() {
  can_motor__motor_speed_feedback_mia();
  TEST_ASSERT_EQUAL_UINT32(motor_wheel_speed_current_val.mia_info.mia_counter, 1000);

  motor_wheel_speed_current_val.mia_info.mia_counter = 0;

  can_motor__motor_speed_feedback_mia();
  TEST_ASSERT_EQUAL_UINT32(motor_wheel_speed_current_val.mia_info.mia_counter, 1000);
}

#endif
