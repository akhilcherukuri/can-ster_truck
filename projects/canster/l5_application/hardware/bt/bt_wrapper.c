#include "bt_wrapper.h"

#include "bt.h"

#include <stdio.h>
#include <string.h>

#include "project.h"

// Get Data
#include "can_driver_node.h"
#include "can_geo_node.h"
#include "can_motor_node.h"

// SENSOR
#include "lidar_data_handler.h"
#include "ultrasonic_wrapper.h"

#include <math.h>

// Const Variables

/**
 * Static state variables
 */
static gps_coordinates_s destination_coordinate;

// SENSOR
static dbc_SENSOR_SONARS_s sensor_sonar;
static dbc_SENSOR_LIDAR_s sensor_lidar;

// GEO
static dbc_GEO_CURRENT_COORDINATES_s current_coordinates;
static dbc_GEO_DEGREE_s geo_degree;
static dbc_GEO_DESTINATION_REACHED_s geo_destination_reached;
static dbc_GEO_DISTANCE_FROM_DESTINATION_s geo_distance_from_destination;

// DEBUG
static dbc_GEO_HEARTBEAT_s geo_checkpoint_index;

// MOTOR
static dbc_MOTOR_SPEED_FEEDBACK_s motor_current_speed;
static dbc_LIPO_BATTERY_VOLTAGE_DBG_s motor_lipo_battery_voltage;
static dbc_MOTOR_INFO_DBG_s motor_info_dbg;

// DRIVER
static dbc_MOTOR_STEERING_s driver_steering;
static dbc_MOTOR_SPEED_s motor_speed_processed;

// BT_WRITE buffer
static char bt_buffer[200];

/**
 * Static function declaration
 */
static void bt_wrapper__read_cb(char *buffer, char *identifier);
static void bt_wrapper__update_decoded_messages(void);
static void bt_wrapper__update_write_buffer(void);

// Static parsers
static void bt_wrapper__parse_loc(char *buffer);

/**
 * Functions
 */
void bt_wrapper__read_once(void) { bt__read(bt_wrapper__read_cb); }

void bt_wrapper__write_once(void) {
  bt_wrapper__update_decoded_messages();

  bt_wrapper__update_write_buffer();
  bt__write(bt_buffer);
}

void bt_wrapper__get_destination_coordinates(gps_coordinates_s *dest_coordinate) {
  *dest_coordinate = destination_coordinate;
}

/**
 * Static functions
 */
static void bt_wrapper__read_cb(char *buffer, char *identifier) {
  // Parse everything here
  if (strcmp(identifier, "$loc") == 0) {
    bt_wrapper__parse_loc(buffer);
  }
}

// TODO, Update more data here
static void bt_wrapper__update_decoded_messages(void) {
  // SENSORS
  ultrasonic__get_distance_from_all_sensors(&sensor_sonar);
  lidar_data_handler__get_distances(&sensor_lidar);

  // CAN NODES
  // MOTOR
  motor_current_speed = can_motor__get_motor_speed_feedback();
  motor_lipo_battery_voltage = can_motor__get_lipo_battery_voltage_debug();
  motor_info_dbg = can_motor__get_motor_info_debug();

  // GEO
  geo_degree = can_geo__get_geo_degree();
  geo_destination_reached = can_geo__get_destination_reached();
  current_coordinates = can_geo__get_current_coordinates();
  geo_distance_from_destination = can_geo__get_distance_from_destination();

  // DEBUG
  geo_checkpoint_index = can_geo__get_heartbeat();

  // DRIVER
  driver_steering = can_driver__get_driver_steering();
  motor_speed_processed = can_driver__get_driver_required_motor_speed();
}

// TODO, Update this when `update_decoded_messages` is done
static void bt_wrapper__update_write_buffer(void) {
  snprintf(bt_buffer, sizeof(bt_buffer) / sizeof(char),
           "$canster,%f,%f,%f,%f,%d,%d,%d,%f,%f,%f,%d,%d,%f,%f,%d,%f,%f,%d,%d\n",
           (double)current_coordinates.GEO_CURRENT_COORDINATES_latitude,
           (double)current_coordinates.GEO_CURRENT_COORDINATES_longitude, (double)destination_coordinate.latitude,
           (double)destination_coordinate.longitude, sensor_lidar.SENSOR_LIDAR_slight_left,
           sensor_lidar.SENSOR_LIDAR_middle, sensor_lidar.SENSOR_LIDAR_slight_right,
           (double)motor_current_speed.MOTOR_SPEED_current, (double)geo_degree.GEO_DEGREE_current,
           (double)geo_degree.GEO_DEGREE_required, driver_steering.MOTOR_STEERING_direction,
           geo_destination_reached.GEO_DESTINATION_REACHED_cmd,
           (double)geo_distance_from_destination.GEO_distance_from_destination,
           (double)motor_lipo_battery_voltage.LIPO_BATTERY_VOLTAGE_val, motor_info_dbg.MOTOR_INFO_DBG_rps,
           (double)motor_info_dbg.MOTOR_INFO_DBG_pwm, (double)sensor_sonar.SENSOR_SONARS_middle,
           motor_speed_processed.MOTOR_SPEED_processed, geo_checkpoint_index.GEO_HEARTBEAT_cmd);
}

/**
 * Static Parsers
 */
static void bt_wrapper__parse_loc(char *buffer) {
  char id[10] = {0};
  sscanf(buffer, "%s %f %f", id, &destination_coordinate.latitude, &destination_coordinate.longitude);

  printf("\n\nDestination coordinates = %lf %lf\r\n", (double)destination_coordinate.latitude,
         (double)destination_coordinate.longitude);
}
