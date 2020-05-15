#include "unity.h"

#include "Mockbt.h"
#include "Mockgps.h"

// Read
#include "Mockcan_driver_node.h"
#include "Mockcan_geo_node.h"
#include "Mockcan_motor_node.h"

#include "Mocklidar_data_handler.h"
#include "Mockultrasonic_wrapper.h"

#include "bt_wrapper.c"

void setUp() {
  destination_coordinate.latitude = 0;
  destination_coordinate.longitude = 0;
}
void tearDown() {}

void test_bt_wrapper__read_once(void) {
  bt__read_Expect(bt_wrapper__read_cb);
  bt_wrapper__read_once();
}

void test_bt_wrapper__get_destination_coordinates(void) {
  destination_coordinate.latitude = 12.23;
  destination_coordinate.longitude = 23.34;

  gps_coordinates_s getter_coord;
  bt_wrapper__get_destination_coordinates(&getter_coord);
  TEST_ASSERT_EQUAL_FLOAT(getter_coord.latitude, 12.23);
  TEST_ASSERT_EQUAL_FLOAT(getter_coord.longitude, 23.34);
}

void test_bt_wrapper__parse_loc_true(void) {
  char buffer[] = "$loc 123.78 234.89";

  bt_wrapper__parse_loc(buffer);

  TEST_ASSERT_EQUAL_FLOAT(destination_coordinate.latitude, 123.78);
  TEST_ASSERT_EQUAL_FLOAT(destination_coordinate.longitude, 234.89);
}

void test_bt_wrapper__parse_loc_false(void) {
  char buffer[] = "$loc 123";

  bt_wrapper__parse_loc(buffer);

  TEST_ASSERT_EQUAL_FLOAT(destination_coordinate.latitude, 123.0);
  TEST_ASSERT_EQUAL_FLOAT(destination_coordinate.longitude, 0.0); // NOTE, This should not happen
}

// TODO, Write these tests for android debugging

void test_bt_wrapper__update_decoded_messages(void) {
  // SENSORS
  ultrasonic__get_distance_from_all_sensors_Expect(&sensor_sonar);
  lidar_data_handler__get_distances_Expect(&sensor_lidar);

  // CAN NODE
  // MOTOR
  can_motor__get_motor_speed_feedback_ExpectAndReturn(motor_current_speed);
  can_motor__get_lipo_battery_voltage_debug_ExpectAndReturn(motor_lipo_battery_voltage);
  can_motor__get_motor_info_debug_ExpectAndReturn(motor_info_dbg);

  // GEO
  can_geo__get_geo_degree_ExpectAndReturn(geo_degree);
  can_geo__get_destination_reached_ExpectAndReturn(geo_destination_reached);
  can_geo__get_current_coordinates_ExpectAndReturn(current_coordinates);
  can_geo__get_distance_from_destination_ExpectAndReturn(geo_distance_from_destination);

  // DRIVER
  can_driver__get_driver_steering_ExpectAndReturn(driver_steering);
  can_driver__get_driver_required_motor_speed_ExpectAndReturn(motor_speed_processed);

  bt_wrapper__update_decoded_messages();
}

void test_bt_wrapper__write_once(void) {
  // SENSORS
  ultrasonic__get_distance_from_all_sensors_Expect(&sensor_sonar);
  lidar_data_handler__get_distances_Expect(&sensor_lidar);

  // CAN NODE
  // MOTOR
  can_motor__get_motor_speed_feedback_ExpectAndReturn(motor_current_speed);
  can_motor__get_lipo_battery_voltage_debug_ExpectAndReturn(motor_lipo_battery_voltage);
  can_motor__get_motor_info_debug_ExpectAndReturn(motor_info_dbg);

  // GEO
  can_geo__get_geo_degree_ExpectAndReturn(geo_degree);
  can_geo__get_destination_reached_ExpectAndReturn(geo_destination_reached);
  can_geo__get_current_coordinates_ExpectAndReturn(current_coordinates);
  can_geo__get_distance_from_destination_ExpectAndReturn(geo_distance_from_destination);

  // DRIVER
  can_driver__get_driver_steering_ExpectAndReturn(driver_steering);
  can_driver__get_driver_required_motor_speed_ExpectAndReturn(motor_speed_processed);

  // MISC
  bt__write_Expect(bt_buffer);

  bt_wrapper__write_once();
}
