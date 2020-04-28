#include "unity.h"

#include "can_mia_configurations.c"
#include "project.h"
#include "who_am_i.h"

#include "Mockcan_handler.h"

#include "Mockboard_io.h"
#include "Mockcompass.h"
#include "Mockdriver_obstacle.h"
#include "Mockgeo_logic.h"
#include "Mockgpio.h"

#include "can_geo_node.c"

void setUp() {
  geo_heartbeat.mia_info.mia_counter = 0;
  geo_degree.mia_info.mia_counter = 0;
  geo_destination.mia_info.mia_counter = 0;
}

void tearDown() {}

/**
 * MIA
 */
void test_can_geo__geo_heartbeat_mia_false() {
  can_geo__geo_heartbeat_mia();
  TEST_ASSERT_EQUAL_INT(geo_heartbeat.mia_info.mia_counter, 1000);

  can_geo__geo_heartbeat_mia();
  TEST_ASSERT_EQUAL_INT(geo_heartbeat.mia_info.mia_counter, 2000);

  geo_heartbeat.mia_info.mia_counter = 0;

  can_geo__geo_heartbeat_mia();
  TEST_ASSERT_EQUAL_INT(geo_heartbeat.mia_info.mia_counter, 1000);
}

void test_can_geo__geo_heartbeat_mia_true() {
  gpio_s gpio_unused;
  board_io__get_led1_ExpectAndReturn(gpio_unused);
  gpio__set_Expect(gpio_unused);

  can_geo__geo_heartbeat_mia();
  TEST_ASSERT_EQUAL_INT(geo_heartbeat.mia_info.mia_counter, 1000);

  can_geo__geo_heartbeat_mia();
  TEST_ASSERT_EQUAL_INT(geo_heartbeat.mia_info.mia_counter, 2000);

  can_geo__geo_heartbeat_mia();
  TEST_ASSERT_EQUAL_INT(geo_heartbeat.mia_info.mia_counter, 3000);
}

void test_can_geo__geo_degree_mia_false() {
  can_geo__geo_degree_mia();
  TEST_ASSERT_EQUAL_INT(geo_degree.mia_info.mia_counter, 1000);

  can_geo__geo_degree_mia();
  TEST_ASSERT_EQUAL_INT(geo_degree.mia_info.mia_counter, 2000);

  geo_degree.mia_info.mia_counter = 0;

  can_geo__geo_degree_mia();
  TEST_ASSERT_EQUAL_INT(geo_degree.mia_info.mia_counter, 1000);
}

void test_can_geo__geo_degree_mia_true() {
  // Run an LED on MIA

  can_geo__geo_degree_mia();
  TEST_ASSERT_EQUAL_INT(geo_degree.mia_info.mia_counter, 1000);

  can_geo__geo_degree_mia();
  TEST_ASSERT_EQUAL_INT(geo_degree.mia_info.mia_counter, 2000);

  can_geo__geo_degree_mia();
  TEST_ASSERT_EQUAL_INT(geo_degree.mia_info.mia_counter, 3000);
}

void test_can__geo_destination_reached_mia_true() {

  can_geo__geo_destination_reached_mia();
  TEST_ASSERT_EQUAL_INT(geo_destination.mia_info.mia_counter, 1000);

  can_geo__geo_destination_reached_mia();
  TEST_ASSERT_EQUAL_INT(geo_destination.mia_info.mia_counter, 2000);

  can_geo__geo_destination_reached_mia();
  TEST_ASSERT_EQUAL_INT(geo_destination.mia_info.mia_counter, 3000);
  TEST_ASSERT_EQUAL_INT(geo_destination.GEO_DESTINATION_REACHED_cmd, GEO_DESTINATION_REACHED_cmd_NOT_REACHED);
}

void test_can__geo_destination_reached_mia_false() {
  can_geo__geo_destination_reached_mia();
  TEST_ASSERT_EQUAL_INT(geo_destination.mia_info.mia_counter, 1000);

  can_geo__geo_destination_reached_mia();
  TEST_ASSERT_EQUAL_INT(geo_destination.mia_info.mia_counter, 2000);

  geo_destination.mia_info.mia_counter = 0;
  can_geo__geo_destination_reached_mia();
  TEST_ASSERT_EQUAL_INT(geo_destination.mia_info.mia_counter, 1000);
}

/**
 * DECODE
 */
void test_can_geo__on_decode_geo_degree(void) {
  dbc_GEO_DEGREE_s geo_degree = {};
  // driver_obstacle__geo_controller_directions_Expect(&geo_degree);
  driver_obstacle__set_geo_controller_direction_Expect(&geo_degree);
  driver_obstacle__move_to_destination_ExpectAndReturn(0);
  can_geo__on_decode_geo_degree();
}

/**
 * TRANSMIT
 */
#if BOARD_GEO_NODE == 1

void test_can_geo__transmit_geo_heartbeat(void) {
  dbc_send_can_message_ExpectAnyArgsAndReturn(true);
  can_geo__transmit_geo_heartbeat();
}

void test_can_geo__transmit_geo_degree(void) {
  dbc_GEO_DEGREE_s message;
  compass__read_compass_bearing_16bit_ExpectAndReturn(message.GEO_DEGREE_current);
  geo_logic__compute_required_bearing_ExpectAndReturn(message.GEO_DEGREE_required);

  dbc_send_can_message_ExpectAnyArgsAndReturn(true);
  can_geo__transmit_geo_degree();
}

#else
void test_can_geo__transmit_geo_heartbeat(void) {}
void test_can_geo__transmit_geo_degree(void) {}
#endif