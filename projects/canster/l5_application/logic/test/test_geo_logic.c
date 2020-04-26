#include "project.h"
#include "unity.h"

#include "Mockgps.h"

#include "geo_logic.c"

void setUp() {
  destination_coordinate.DRIVER_COORDINATES_latitude = 0;
  destination_coordinate.DRIVER_COORDINATES_longitude = 0;
}

void tearDown() {}

void test_geo_logic__update_destination_coordinate(void) {
  dbc_DRIVER_COORDINATES_s driver_coordinates = {};
  driver_coordinates.DRIVER_COORDINATES_latitude = 2.2789;
  driver_coordinates.DRIVER_COORDINATES_longitude = 1.1234;

  geo_logic__update_destination_coordinate(&driver_coordinates);

  TEST_ASSERT_EQUAL_FLOAT(destination_coordinate.DRIVER_COORDINATES_latitude,
                          driver_coordinates.DRIVER_COORDINATES_latitude);
  TEST_ASSERT_EQUAL_FLOAT(destination_coordinate.DRIVER_COORDINATES_longitude,
                          driver_coordinates.DRIVER_COORDINATES_longitude);
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
