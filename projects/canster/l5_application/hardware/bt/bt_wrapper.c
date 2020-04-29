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
#include "ultrasonic_wrapper.h"

#include <math.h>

// Const Variables
static const float PI = 3.141592654;

/**
 * Static state variables
 */
static float distance_between_coordinates;

static gps_coordinates_s destination_coordinate;

// SENSOR
static dbc_SENSOR_SONARS_s sensor_sonar;

// GEO
static dbc_GEO_CURRENT_COORDINATES_s current_coordinates;
static dbc_GEO_DEGREE_s geo_degree;
static dbc_GEO_DESTINATION_REACHED_s geo_destination_reached;

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

static float bt_wrapper__compute_distance();
static float bt_wrapper__degree_to_radian(float degree);
static float bt_wrapper__radian_to_degree(float radian);

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
  geo_destination_reached = can_geo__get_destination_reached();

  distance_between_coordinates = bt_wrapper__compute_distance();
}

// TODO, Update this when `update_decoded_messages` is done
static void bt_wrapper__update_write_buffer(void) {
  snprintf(bt_buffer, sizeof(bt_buffer) / sizeof(char), "$canster,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%d,%f\n",
           (double)current_coordinates.GEO_CURRENT_COORDINATES_latitude,
           (double)current_coordinates.GEO_CURRENT_COORDINATES_longitude, (double)destination_coordinate.latitude,
           (double)destination_coordinate.longitude, (double)sensor_sonar.SENSOR_SONARS_left,
           (double)sensor_sonar.SENSOR_SONARS_middle, (double)sensor_sonar.SENSOR_SONARS_right,
           (double)motor_current_speed.MOTOR_SPEED_current, (double)geo_degree.GEO_DEGREE_current,
           (double)geo_degree.GEO_DEGREE_required, driver_steering.MOTOR_STEERING_direction,
           geo_destination_reached.GEO_DESTINATION_REACHED_cmd, (double)distance_between_coordinates);
}

/**
 * Static Parsers
 */
static void bt_wrapper__parse_loc(char *buffer) {
  char id[10] = {0};
  sscanf(buffer, "%s %f %f", id, &destination_coordinate.latitude, &destination_coordinate.longitude);

  printf("%f %f\r\n", (double)destination_coordinate.latitude, (double)destination_coordinate.longitude);
}

/**
 * STATIC FUNCTIONS
 */
static float bt_wrapper__compute_distance() {
  static const uint32_t EARTH_RADIUS = 6371 * 1000;

  float current_coordinate_latitude_radian =
      bt_wrapper__degree_to_radian(current_coordinates.GEO_CURRENT_COORDINATES_latitude);
  float destination_coordinate_latitude_radian = bt_wrapper__degree_to_radian(destination_coordinate.latitude);

  float delta_latitude = bt_wrapper__degree_to_radian(destination_coordinate.latitude -
                                                      current_coordinates.GEO_CURRENT_COORDINATES_latitude);
  float delta_longitude = bt_wrapper__degree_to_radian(destination_coordinate.longitude -
                                                       current_coordinates.GEO_CURRENT_COORDINATES_longitude);

  float a = sinf(delta_latitude / 2) * sinf(delta_latitude / 2) +
            cosf(current_coordinate_latitude_radian) * cosf(destination_coordinate_latitude_radian) *
                sinf(delta_longitude / 2) * sinf(delta_longitude / 2);

  float c = 2 * atan2f(sqrtf(a), sqrtf(1 - a));
  float d = EARTH_RADIUS * c;
  return d;
}

static float bt_wrapper__degree_to_radian(float degree) {
  float rval = (degree * PI) / 180;
  return rval;
}

static float bt_wrapper__radian_to_degree(float radian) {
  float rval = (radian * 180) / PI;
  return rval;
}
