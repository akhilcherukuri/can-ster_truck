#include "project.h"
#include "unity.h"

#include "Mockgps.h"

#include "geo_logic.c"

void setUp() {
  destination_coordinate.SENSOR_BT_COORDINATES_latitude = 0;
  destination_coordinate.SENSOR_BT_COORDINATES_longitude = 0;
}

void tearDown() {}

void test_geo_logic__update_destination_coordinate(void) {
  dbc_SENSOR_BT_COORDINATES_s driver_coordinates = {};
  driver_coordinates.SENSOR_BT_COORDINATES_latitude = 2.2789;
  driver_coordinates.SENSOR_BT_COORDINATES_longitude = 1.1234;

  geo_logic__update_destination_coordinate(&driver_coordinates);

  TEST_ASSERT_EQUAL_FLOAT(destination_coordinate.SENSOR_BT_COORDINATES_latitude,
                          driver_coordinates.SENSOR_BT_COORDINATES_latitude);
  TEST_ASSERT_EQUAL_FLOAT(destination_coordinate.SENSOR_BT_COORDINATES_longitude,
                          driver_coordinates.SENSOR_BT_COORDINATES_longitude);
}

// TODO,

void test_geo_logic__compute_required_bearing_dont_move(void) {
  gps_coordinates_s coordinates = {};
  coordinates.latitude = 10.2;
  coordinates.longitude = 20.3;
  gps__get_coordinates_ExpectAndReturn(coordinates);

  destination_coordinate.DRIVER_COORDINATES_latitude = 10.2;
  destination_coordinate.DRIVER_COORDINATES_longitude = 20.3;

  float bearing = geo_logic__compute_required_bearing();
  TEST_ASSERT_EQUAL_FLOAT(bearing, 180);
}

void test_geo_logic__compute_required_bearing(void) {
  gps_coordinates_s coordinates = {};
  coordinates.latitude = 5.2;
  coordinates.longitude = 10.3;
  gps__get_coordinates_ExpectAndReturn(coordinates);

  destination_coordinate.DRIVER_COORDINATES_latitude = 10.2;
  destination_coordinate.DRIVER_COORDINATES_longitude = 20.3;

  float bearing = geo_logic__compute_required_bearing();
  TEST_ASSERT_EQUAL_FLOAT(bearing, 242);
}

/**
 * San Jose
 * 37.338207
 * -121.886330
 *
 * SJSU
 * 37.441810
 * -122.165280
 */
void test_geo_logic__compute_required_bearing_real(void) {
  gps_coordinates_s coordinates = {};
  coordinates.latitude = 37.338207;
  coordinates.longitude = -121.886330;
  gps__get_coordinates_ExpectAndReturn(coordinates);

  destination_coordinate.DRIVER_COORDINATES_latitude = 37.441810;
  destination_coordinate.DRIVER_COORDINATES_longitude = -122.165280;

  float bearing = geo_logic__compute_required_bearing();
  TEST_ASSERT_EQUAL_FLOAT(bearing, 108);
}

void test_geo_logic__distance_from_destination_real(void) {
  gps_coordinates_s coordinates = {};
  coordinates.latitude = 37.338207;
  coordinates.longitude = -121.886330;
  gps__get_coordinates_ExpectAndReturn(coordinates);

  destination_coordinate.DRIVER_COORDINATES_latitude = 37.441810;
  destination_coordinate.DRIVER_COORDINATES_longitude = -122.165280;

  float distance = geo_logic__distance_from_destination();
  TEST_ASSERT_EQUAL_FLOAT(distance, 27204.23); // meters
}

void test_geo_logic__distance_from_destination_reached(void) {
  gps_coordinates_s coordinates = {};
  coordinates.latitude = 37.441810;
  coordinates.longitude = -122.165280;
  gps__get_coordinates_ExpectAndReturn(coordinates);

  destination_coordinate.DRIVER_COORDINATES_latitude = 37.441810;
  destination_coordinate.DRIVER_COORDINATES_longitude = -122.165280;

  float distance = geo_logic__distance_from_destination();
  TEST_ASSERT_EQUAL_FLOAT(distance, 0); // meters
}

void test_geo_logic__distance_from_destination_tiny_increment(void) {
  gps_coordinates_s coordinates = {};
  coordinates.latitude = 37.441813; // step change by 3
  coordinates.longitude = -122.165280;
  gps__get_coordinates_ExpectAndReturn(coordinates);

  destination_coordinate.DRIVER_COORDINATES_latitude = 37.441810;
  destination_coordinate.DRIVER_COORDINATES_longitude = -122.165280;

  float distance = geo_logic__distance_from_destination();
  TEST_ASSERT_EQUAL_FLOAT(distance, 0.424175); // meters
}

void test_geo_logic__compute_destination_reached_true() {
  gps_coordinates_s coordinates = {};
  coordinates.latitude = 37.441813; // step change by 3
  coordinates.longitude = -122.165280;
  gps__get_coordinates_ExpectAndReturn(coordinates);

  destination_coordinate.DRIVER_COORDINATES_latitude = 37.441810;
  destination_coordinate.DRIVER_COORDINATES_longitude = -122.165280;

  dbc_GEO_DESTINATION_REACHED_s destination_reached = geo_logic__compute_destination_reached();

  TEST_ASSERT_EQUAL_UINT8(destination_reached.GEO_DESTINATION_REACHED_cmd, GEO_DESTINATION_REACHED_cmd_REACHED);
}

void test_geo_logic__compute_destination_reached_false() {
  gps_coordinates_s coordinates = {};
  coordinates.latitude = 37.441823; // step change by 23
  coordinates.longitude = -122.165280;
  gps__get_coordinates_ExpectAndReturn(coordinates);

  destination_coordinate.DRIVER_COORDINATES_latitude = 37.441869;
  destination_coordinate.DRIVER_COORDINATES_longitude = -122.165326;

  dbc_GEO_DESTINATION_REACHED_s destination_reached = geo_logic__compute_destination_reached();

  TEST_ASSERT_EQUAL_UINT8(destination_reached.GEO_DESTINATION_REACHED_cmd, GEO_DESTINATION_REACHED_cmd_NOT_REACHED);
}