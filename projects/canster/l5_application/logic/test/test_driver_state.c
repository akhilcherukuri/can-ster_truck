#include "unity.h"

#include "project.h"

#include "driver_state.c"

void setUp() {
  destination_coordinate.DRIVER_COORDINATES_latitude = 0;
  destination_coordinate.DRIVER_COORDINATES_longitude = 0;
}
void tearDown() {}

void test_driver_state__update_destination_coordinate(void) {
  dbc_DRIVER_COORDINATES_s coordinates = {{0}, 1.1, 2.2};
  driver_state__update_destination_coordinate(&coordinates);

  TEST_ASSERT_EQUAL_FLOAT(coordinates.DRIVER_COORDINATES_latitude, destination_coordinate.DRIVER_COORDINATES_latitude);
  TEST_ASSERT_EQUAL_FLOAT(coordinates.DRIVER_COORDINATES_longitude,
                          destination_coordinate.DRIVER_COORDINATES_longitude);
}

void test_driver_state__get_destination_coordinate(void) {
  dbc_DRIVER_COORDINATES_s coordinates = {{0}, 1.1, 2.2};
  driver_state__update_destination_coordinate(&coordinates);

  const dbc_DRIVER_COORDINATES_s *coordinate_ref = driver_state__get_destination_coordinate();

  TEST_ASSERT_EQUAL_FLOAT(coordinates.DRIVER_COORDINATES_latitude, coordinate_ref->DRIVER_COORDINATES_latitude);
  TEST_ASSERT_EQUAL_FLOAT(coordinates.DRIVER_COORDINATES_longitude, coordinate_ref->DRIVER_COORDINATES_longitude);
}