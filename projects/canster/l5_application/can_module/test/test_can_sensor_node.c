#include "unity.h"

#include "can_mia_configurations.c"
#include "project.h"

#include "Mockboard_io.h"
#include "Mockcan_handler.h"
#include "Mockdriver_obstacle.h"
#include "Mockgpio.h"
#include "Mockultrasonic_wrapper.h"

#include "Mocklcd_ui.h"

#include "can_sensor_node.c"
#include "who_am_i.h"

void setUp() {
  sensor_heartbeat.mia_info.mia_counter = 0;
  sensor_sonar.mia_info.mia_counter = 0;
}

void test__can_sensor_heartbeat_mia_true(void) {

  set_lcd_mia_led_Expect(SENSOR_LED_MIA, false);

  gpio_s gpio_unused = {};
  gpio__set_Expect(gpio_unused);
  board_io__get_led3_ExpectAndReturn(gpio_unused);

  for (int i = 0; i < 3000; i += 1000) {
    can_sensor__sensor_heartbeat_mia();
    TEST_ASSERT_EQUAL_UINT32(sensor_heartbeat.mia_info.mia_counter, i + 1000);
  }

  TEST_ASSERT(sensor_heartbeat.SENSOR_HEARTBEAT_cmd == SENSOR_HEARTBEAT_cmd_REBOOT);
}

void test__can_sensor_heartbeat_mia_true_false(void) {
  can_sensor__sensor_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(sensor_heartbeat.mia_info.mia_counter, 1000);

  can_sensor__sensor_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(sensor_heartbeat.mia_info.mia_counter, 2000);

  sensor_heartbeat.mia_info.mia_counter = 0;
  can_sensor__sensor_heartbeat_mia();
  TEST_ASSERT_EQUAL_UINT32(sensor_heartbeat.mia_info.mia_counter, 1000);
}

void test_can_sensor_sonar_mia_true(void) {
  driver_obstacle__process_input_Expect(&sensor_sonar);

  for (int i = 0; i < 3000; i += 1000) {
    can_sensor__sensor_sonar_mia();
    TEST_ASSERT_EQUAL_UINT32(sensor_sonar.mia_info.mia_counter, i + 1000);
  }

  TEST_ASSERT_EQUAL_UINT32(sensor_sonar.SENSOR_SONARS_left, 0);
  TEST_ASSERT_EQUAL_UINT32(sensor_sonar.SENSOR_SONARS_middle, 0);
  TEST_ASSERT_EQUAL_UINT32(sensor_sonar.SENSOR_SONARS_right, 0);
}

#if BOARD_SENSOR_NODE == 1
void test_can_sensor__transmit_all_messages(void) {
  dbc_SENSOR_SONARS_s sensor_sonar_data_unused = {};
  dbc_send_can_message_ExpectAnyArgsAndReturn(true);
  dbc_send_can_message_ExpectAnyArgsAndReturn(true);
  ultrasonic__get_distance_from_all_sensors_Expect(&sensor_sonar_data_unused);

  can_sensor__transmit_all_messages();
}
#else
void test_can_sensor__transmit_all_messages(void) {}
#endif
