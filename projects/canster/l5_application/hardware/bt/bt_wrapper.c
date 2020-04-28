#include "bt_wrapper.h"

#include "bt.h"

#include <stdio.h>
#include <string.h>

// Get Data
#include "can_driver_node.h"
#include "can_geo_node.h"
#include "can_motor_node.h"

// SENSOR
#include "ultrasonic_wrapper.h"

/**
 * Static state variables
 */
static gps_coordinates_s destination_coordinate;
static dbc_SENSOR_SONARS_s sensor_sonar;

// GEO
static dbc_GEO_CURRENT_COORDINATES_s current_coordinates;
static dbc_GEO_DEGREE_s geo_degree;

// MOTOR
static dbc_MOTOR_SPEED_FEEDBACK_s motor_current_speed;

// DRIVER
static dbc_MOTOR_STEERING_s driver_steering;

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
  current_coordinates = can_geo__get_current_coordinates();
  ultrasonic__get_distance_from_all_sensors(&sensor_sonar);

  motor_current_speed = can_motor__get_motor_speed_feedback();
  geo_degree = can_geo__get_geo_degree();

  driver_steering = can_driver__get_driver_steering();
}

// TODO, Update this when `update_decoded_messages` is done
static void bt_wrapper__update_write_buffer(void) {
  snprintf(bt_buffer, sizeof(bt_buffer) / sizeof(char), "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d\r\n",
           (double)current_coordinates.GEO_CURRENT_COORDINATES_latitude,
           (double)current_coordinates.GEO_CURRENT_COORDINATES_longitude, (double)destination_coordinate.latitude,
           (double)destination_coordinate.longitude, (double)sensor_sonar.SENSOR_SONARS_left,
           (double)sensor_sonar.SENSOR_SONARS_middle, (double)sensor_sonar.SENSOR_SONARS_right,
           (double)motor_current_speed.MOTOR_SPEED_current, (double)geo_degree.GEO_DEGREE_current,
           (double)geo_degree.GEO_DEGREE_required, driver_steering.MOTOR_STEERING_direction);
}

/**
 * Static Parsers
 */
static void bt_wrapper__parse_loc(char *buffer) {
  char id[10] = {0};
  sscanf(buffer, "%s %f %f", id, &destination_coordinate.latitude, &destination_coordinate.longitude);

  printf("%f %f\r\n", (double)destination_coordinate.latitude, (double)destination_coordinate.longitude);
}
