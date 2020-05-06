#include "unity.h"

#include "can_mia_configurations.c"
#include "project.h"

#include "Mockboard_io.h"
#include "Mockcan_handler.h"
#include "Mockdriver_obstacle.h"
#include "Mockgpio.h"
#include "Mocklidar_data_handler.h"
#include "Mockultrasonic_wrapper.h"

#include "Mockbt_wrapper.h"
#include "Mockgps.h"

#include "can_sensor_node.c"
#include "who_am_i.h"

void setUp() {
  sensor_heartbeat.mia_info.mia_counter = 0;
  sensor_sonar.mia_info.mia_counter = 0;
}

void test__can_sensor_heartbeat_mia_true(void) {
  gpio_s gpio_unused = {};
  gpio__set_Expect(gpio_unused);
  board_io__get_led3_ExpectAndReturn(gpio_unused);
  for (int i = 100; i <= 3000; i += 100) {
    can_sensor__sensor_heartbeat_mia();
    TEST_ASSERT_EQUAL_UINT32(sensor_heartbeat.mia_info.mia_counter, i);
  }
  TEST_ASSERT(sensor_heartbeat.SENSOR_HEARTBEAT_cmd == SENSOR_HEARTBEAT_cmd_REBOOT);
}

void test__can_sensor_heartbeat_mia_true_false(void) {

  can_sensor__sensor_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(sensor_heartbeat.mia_info.mia_counter, 100);

  can_sensor__sensor_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(sensor_heartbeat.mia_info.mia_counter, 200);

  sensor_heartbeat.mia_info.mia_counter = 0;

  can_sensor__sensor_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(sensor_heartbeat.mia_info.mia_counter, 100);
}

void test_can_sensor_sonar_mia_true(void) {
  driver_obstacle__process_input_Expect(&sensor_sonar);
  gpio_s gpio_unused = {};
  gpio__set_Expect(gpio_unused);
  board_io__get_led2_ExpectAndReturn(gpio_unused);

  for (int i = 100; i <= 3000; i += 100) {
    can_sensor__sensor_sonar_mia();
    TEST_ASSERT_EQUAL_UINT32(sensor_sonar.mia_info.mia_counter, i);
  }
  TEST_ASSERT_EQUAL_UINT32(sensor_sonar.SENSOR_SONARS_left, 0);
  TEST_ASSERT_EQUAL_UINT32(sensor_sonar.SENSOR_SONARS_middle, 0);
  TEST_ASSERT_EQUAL_UINT32(sensor_sonar.SENSOR_SONARS_right, 0);
}

#if BOARD_SENSOR_NODE == 1

void test_can_sensor__transmit_sensor_bt_coordinates() {
  gps_coordinates_s dest_coordinates = {};
  bt_wrapper__get_destination_coordinates_Expect(NULL);
  bt_wrapper__get_destination_coordinates_IgnoreArg_dest_coordinate();
  bt_wrapper__get_destination_coordinates_ReturnThruPtr_dest_coordinate(&dest_coordinates);

  dbc_send_can_message_ExpectAnyArgsAndReturn(true);

  can_sensor__transmit_sensor_bt_coordinates();
}

#else
void test_can_sensor__transmit_sensor_bt_coordinates();
#endif
