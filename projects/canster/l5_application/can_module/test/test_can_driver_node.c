#include "unity.h"

#include "can_mia_configurations.c"
#include "project.h"

#include "Mockboard_io.h"
#include "Mockcan_handler.h"
#include "Mockgpio.h"

#include "Mockdriver_obstacle.h"
#include "Mockdriver_state.h"

#include "can_driver_node.c"
#include "who_am_i.h"

void setUp() {
  driver_steering.mia_info.mia_counter = 0;
  driver_required_motor_speed.mia_info.mia_counter = 0;
  driver_heartbeat.mia_info.mia_counter = 0;
}
void tearDown() {}

/**
 * MIA
 */
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
  // gpio_s gpio;
  // board_io__get_led2_ExpectAndReturn(gpio);
  // gpio__set_Expect(gpio);

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
  // gpio_s gpio;
  // board_io__get_led2_ExpectAndReturn(gpio);
  // gpio__set_Expect(gpio);

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

#if BOARD_DRIVER_NODE == 1
/**
 * Static
 */
void test_can_driver__transmit_driver_steering() {
  dbc_MOTOR_STEERING_s message;
  driver_obstacle__get_motor_commands_ExpectAndReturn(message);
  dbc_send_can_message_ExpectAnyArgsAndReturn(true);

  can_driver__transmit_driver_steering();
}

void test_can_driver__transmit_driver_required_motor_speed() {
  dbc_send_can_message_ExpectAnyArgsAndReturn(true);
  can_driver__transmit_driver_required_motor_speed();
}

void test_can_driver__transmit_driver_heartbeat() {
  dbc_send_can_message_ExpectAnyArgsAndReturn(true);
  can_driver__transmit_driver_heartbeat();
}

void test_can_driver__transmit_driver_coordinates() {
  const dbc_DRIVER_COORDINATES_s destination_coordinate;
  driver_state__get_destination_coordinate_ExpectAndReturn(&destination_coordinate);
  dbc_send_can_message_ExpectAnyArgsAndReturn(true);
  can_driver__transmit_driver_coordinates();
}

#else
void test_can_driver__transmit_driver_steering() {}
void test_can_driver__transmit_driver_required_motor_speed() {}
void test_can_driver__transmit_driver_heartbeat() {}
void test_can_driver__transmit_driver_coordinates() {}
#endif

/**
 * DECODE
 */
void test_can_driver__on_decode_driver_coordinates(void) { can_driver__on_decode_driver_coordinates(); }
