#include "unity.h"

#include "can_mia_configurations.c"
#include "project.h"

#include "Mockboard_io.h"
#include "Mockcan_handler.h"
#include "Mockdriver_obstacle.h"
#include "Mockgpio.h"

#include "can_sensor_node.c"
#include "who_am_i.h"

void setUp() {
  sensor_heartbeat.mia_info.mia_counter = 0;
  sensor_sonar.mia_info.mia_counter = 0;
}

void test_can_sensor__sensor_heartbeat_mia_true(void) {
  gpio_s gpio;
  board_io__get_led2_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);

  can_sensor__sensor_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(sensor_heartbeat.mia_info.mia_counter, 1000);

  can_sensor__sensor_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(sensor_heartbeat.mia_info.mia_counter, 2000);

  can_sensor__sensor_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(sensor_heartbeat.mia_info.mia_counter, 3000);
  TEST_ASSERT(sensor_heartbeat.SENSOR_HEARTBEAT_cmd == SENSOR_HEARTBEAT_cmd_REBOOT);
}

void test_can_sensor__sensor_heartbeat_mia_true_false(void) {
  can_sensor__sensor_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(sensor_heartbeat.mia_info.mia_counter, 1000);

  can_sensor__sensor_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(sensor_heartbeat.mia_info.mia_counter, 2000);

  sensor_heartbeat.mia_info.mia_counter = 0;
  can_sensor__sensor_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(sensor_heartbeat.mia_info.mia_counter, 1000);
}

void test_can_sensor__sensor_sonar_mia_true(void) {
  gpio_s gpio;
  board_io__get_led2_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);

  driver_obstacle__process_input_Expect(&sensor_sonar);

  can_sensor__sensor_sonar_mia();
  TEST_ASSERT_EQUAL_UINT32(sensor_sonar.mia_info.mia_counter, 1000);

  can_sensor__sensor_sonar_mia();
  TEST_ASSERT_EQUAL_UINT32(sensor_sonar.mia_info.mia_counter, 2000);

  can_sensor__sensor_sonar_mia();
  TEST_ASSERT_EQUAL_UINT32(sensor_sonar.mia_info.mia_counter, 3000);
  TEST_ASSERT(sensor_sonar.SENSOR_SONARS_left == 10);
  TEST_ASSERT(sensor_sonar.SENSOR_SONARS_middle == 10);
  TEST_ASSERT(sensor_sonar.SENSOR_SONARS_right == 10);
}

void test_can_sensor__sensor_sonar_mia_false(void) {
  can_sensor__sensor_sonar_mia();
  TEST_ASSERT_EQUAL_UINT32(sensor_sonar.mia_info.mia_counter, 1000);

  can_sensor__sensor_sonar_mia();
  TEST_ASSERT_EQUAL_UINT32(sensor_sonar.mia_info.mia_counter, 2000);

  sensor_sonar.mia_info.mia_counter = 0;
  can_sensor__sensor_sonar_mia();
  TEST_ASSERT_EQUAL_UINT32(sensor_sonar.mia_info.mia_counter, 1000);
}

#if BOARD_SENSOR_NODE

void test_can_sensor__transmit_all_messages(void) {
  dbc_send_can_message_ExpectAnyArgsAndReturn(true);
  dbc_send_can_message_ExpectAnyArgsAndReturn(true);

  can_sensor__transmit_all_messages();
}
#else
void test_can_sensor__transmit_all_messages(void) {}
#endif