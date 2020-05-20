#include "project.h"
#include "unity.h"

#include "Mockgps.h"

#include "geo_logic.c"

void setUp() {
  destination_gps_coordinate.latitude = 0;
  destination_gps_coordinate.longitude = 0;

  current_checkpoint.latitude = 0;
  current_checkpoint.longitude = 0;

  current_coordinate.latitude = 0;
  current_coordinate.longitude = 0;
}

void tearDown() {}

void test_geo_logic__update_destination_coordinate(void) {
  // Should be set
  current_coordinate = CHECKPOINTS[7];

  dbc_SENSOR_BT_COORDINATES_s driver_coordinates = {};
  driver_coordinates.SENSOR_BT_COORDINATES_latitude = CHECKPOINTS[3].latitude;
  driver_coordinates.SENSOR_BT_COORDINATES_longitude = CHECKPOINTS[3].longitude;

  geo_logic__update_destination_coordinate(&driver_coordinates);

  TEST_ASSERT_EQUAL_FLOAT(destination_gps_coordinate.latitude, driver_coordinates.SENSOR_BT_COORDINATES_latitude);
  TEST_ASSERT_EQUAL_FLOAT(destination_gps_coordinate.longitude, driver_coordinates.SENSOR_BT_COORDINATES_longitude);

  TEST_ASSERT(current_checkpoint.latitude == CHECKPOINTS[5].latitude);
  TEST_ASSERT(current_checkpoint.longitude == CHECKPOINTS[5].longitude);
}

void test_geo_logic__compute_required_bearing_dont_move(void) {
  destination_gps_coordinate = CHECKPOINTS[5];

  current_checkpoint = CHECKPOINTS[5];
  current_coordinate = CHECKPOINTS[5];

  float bearing = geo_logic__compute_required_bearing();
  TEST_ASSERT_EQUAL_FLOAT(bearing, 180);
}

void test_geo_logic__compute_required_bearing(void) {
  destination_gps_coordinate = CHECKPOINTS[2];

  // 5.2, 20.3
  current_coordinate.latitude = 5.2;
  current_coordinate.longitude = 10.3;

  // 10.2, 20.3
  current_checkpoint.latitude = 10.2;
  current_checkpoint.longitude = 20.3;

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
  destination_gps_coordinate = CHECKPOINTS[2];

  current_coordinate.latitude = 37.338207;
  current_coordinate.longitude = -121.886330;

  current_checkpoint.latitude = 37.441810;
  current_checkpoint.longitude = -122.165280;

  float bearing = geo_logic__compute_required_bearing();
  TEST_ASSERT_EQUAL_FLOAT(bearing, 108);
}

void test_geo_logic__distance_from_destination_real(void) {
  current_coordinate.latitude = 37.338207;
  current_coordinate.longitude = -121.886330;

  destination_gps_coordinate.latitude = 37.441810;
  destination_gps_coordinate.longitude = -122.165280;

  float distance = geo_logic__distance_from_destination();
  TEST_ASSERT_EQUAL_FLOAT(distance, 27204.23); // meters
}

void test_geo_logic__distance_from_destination_reached(void) {
  current_coordinate.latitude = 37.441810;
  current_coordinate.longitude = -122.165280;

  destination_gps_coordinate.latitude = 37.441810;
  destination_gps_coordinate.longitude = -122.165280;

  float distance = geo_logic__distance_from_destination();
  TEST_ASSERT_EQUAL_FLOAT(distance, 0); // meters
}

void test_geo_logic__distance_from_destination_tiny_increment(void) {
  current_coordinate.latitude = 37.441813; // step change by 3
  current_coordinate.longitude = -122.165280;

  destination_gps_coordinate.latitude = 37.441810;
  destination_gps_coordinate.longitude = -122.165280;

  float distance = geo_logic__distance_from_destination();
  TEST_ASSERT_EQUAL_FLOAT(distance, 0.424175); // meters
}

void test_geo_logic__compute_destination_reached_true() {
  current_coordinate.latitude = 37.441813; // step change by 3
  current_coordinate.longitude = -122.165280;

  destination_gps_coordinate.latitude = 37.441810;
  destination_gps_coordinate.longitude = -122.165280;

  dbc_GEO_DESTINATION_REACHED_s destination_reached = geo_logic__compute_destination_reached();

  TEST_ASSERT_EQUAL_UINT8(destination_reached.GEO_DESTINATION_REACHED_cmd, GEO_DESTINATION_REACHED_cmd_REACHED);
}

void test_geo_logic__compute_destination_reached_false() {
  current_coordinate.latitude = 37.441823; // step change by 23
  current_coordinate.longitude = -122.165280;

  destination_gps_coordinate.latitude = 37.441869;
  destination_gps_coordinate.longitude = -122.165326;

  dbc_GEO_DESTINATION_REACHED_s destination_reached = geo_logic__compute_destination_reached();

  TEST_ASSERT_EQUAL_UINT8(destination_reached.GEO_DESTINATION_REACHED_cmd, GEO_DESTINATION_REACHED_cmd_NOT_REACHED);
}

/**
 * geo_logic__compute_next_checkpoint
 */
#include <stdio.h>
void test_geo_logic__compute_next_checkpoint() {
  destination_gps_coordinate = CHECKPOINTS[2];
  current_coordinate = CHECKPOINTS[7];

  current_checkpoint = geo_logic__compute_next_checkpoint();

  // Should go to checkpoints 13
  TEST_ASSERT(current_checkpoint.latitude == CHECKPOINTS[5].latitude);
  TEST_ASSERT(current_checkpoint.longitude == CHECKPOINTS[5].longitude);
}

void test_geo_logic__compute_next_checkpoint_destination_next() {
  destination_gps_coordinate = CHECKPOINTS[2];
  current_coordinate = CHECKPOINTS[1];

  current_checkpoint = geo_logic__compute_next_checkpoint();

  // Should go to checkpoints 13
  TEST_ASSERT(current_checkpoint.latitude == CHECKPOINTS[2].latitude);
  TEST_ASSERT(current_checkpoint.longitude == CHECKPOINTS[2].longitude);
}

void test_geo_logic__compute_next_checkpoint_destination_reached() {
  destination_gps_coordinate = CHECKPOINTS[5];
  current_coordinate = CHECKPOINTS[5];

  current_checkpoint = geo_logic__compute_next_checkpoint();

  TEST_ASSERT(current_checkpoint.latitude == CHECKPOINTS[5].latitude);
  TEST_ASSERT(current_checkpoint.longitude == CHECKPOINTS[5].longitude);
}

/**
 * TODO, REMOVE THIS LATER
 * TESTS FOR THE 10th Street Garage
 */
void test_geo_logic__complete_navigation_p1() {
  destination_gps_coordinate = CHECKPOINTS[0];
  current_coordinate = CHECKPOINTS[7];
  current_checkpoint = CHECKPOINTS[5];

  float bearing = geo_logic__compute_required_bearing();
  TEST_ASSERT_EQUAL_FLOAT(bearing, 197);
}

void test_geo_logic__complete_navigation_p2() {
  destination_gps_coordinate = CHECKPOINTS[0];
  current_coordinate = CHECKPOINTS[5];
  current_checkpoint = CHECKPOINTS[3];

  bool checkpointreached = geo_logic__checkpoint_reached();
  TEST_ASSERT_FALSE(checkpointreached);

  float bearing = geo_logic__compute_required_bearing();
  TEST_ASSERT_EQUAL_FLOAT(bearing, 122);
}

void test_geo_logic__complete_navigation_p3() {
  destination_gps_coordinate = CHECKPOINTS[0];
  current_coordinate = CHECKPOINTS[3];
  current_checkpoint = CHECKPOINTS[3];

  bool checkpointreached = geo_logic__checkpoint_reached();
  TEST_ASSERT_TRUE(checkpointreached);

  float bearing = geo_logic__compute_required_bearing();
  TEST_ASSERT_EQUAL_FLOAT(bearing, 169);
}

void test_geo_logic__complete_navigation_p4() {
  destination_gps_coordinate = CHECKPOINTS[0];
  current_coordinate = CHECKPOINTS[2];
  current_checkpoint = CHECKPOINTS[2];

  float bearing = geo_logic__compute_required_bearing();
  TEST_ASSERT_EQUAL_FLOAT(bearing, 96);
}

void test_geo_logic__complete_navigation_p5() {
  destination_gps_coordinate = CHECKPOINTS[0];
  current_coordinate = CHECKPOINTS[0];
  current_checkpoint = CHECKPOINTS[0];
  bool checkpointreached = geo_logic__checkpoint_reached();
  TEST_ASSERT_TRUE(checkpointreached);

  float bearing = geo_logic__compute_required_bearing();
  TEST_ASSERT_EQUAL_FLOAT(bearing, 180);

  dbc_GEO_DESTINATION_REACHED_s destination_reached = geo_logic__compute_destination_reached();
  TEST_ASSERT_EQUAL_UINT8(destination_reached.GEO_DESTINATION_REACHED_cmd, GEO_DESTINATION_REACHED_cmd_REACHED);
}
