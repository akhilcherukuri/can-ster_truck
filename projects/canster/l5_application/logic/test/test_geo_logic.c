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
void test_geo_logic__compute_required_bearing(void) {}