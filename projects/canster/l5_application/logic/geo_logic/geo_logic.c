#include "geo_logic.h"

#include <math.h>

#include "gps.h"
#include <stdio.h>

/**
 * STATIC FUNCTIONS
 */

static gps_coordinates_s geo_logic__compute_next_checkpoint();

static bool geo_logic__checkpoint_reached();
static float geo_logic__distance_between_two_coordinates(const gps_coordinates_s *source_coordinates,
                                                         const gps_coordinates_s *destination_coordinates);
static float geo_logic__degree_to_radian(float degree);
static float geo_logic__radian_to_degree(float radian);

/**
 * STATIC CONST VARIABLES
 */
static const float PI = 3.141592654;
static const uint32_t EARTH_RADIUS = 6371 * 1000;
static const float DESTINATION_REACHED_RADIUS_METERS = 4.0;

static const gps_coordinates_s CHECKPOINTS[] = {
    {37.33925, -121.88125}, {37.3394, -121.88124},  {37.33962, -121.8815},  {37.33979, -121.88139},
    {37.33994, -121.88093}, {37.33965, -121.88058}, {37.33952, -121.88083}, {37.33954, -121.88107},
    {37.33916, -121.88108}, {37.33931, -121.88073}, {37.33919, -121.88028}, {37.33902, -121.8805},
    {37.33881, -121.88085}, {37.33865, -121.88073}, {37.3388, -121.88038},
};

/**
 * STATE VARIABLES
 */
// static dbc_SENSOR_BT_COORDINATES_s destination_coordinate = {{0}, 37.340679, -121.894371}; // Uncomment for testing
// static dbc_SENSOR_BT_COORDINATES_s destination_coordinate;
static gps_coordinates_s destination_gps_coordinate;
static gps_coordinates_s current_checkpoint;
static gps_coordinates_s current_coordinate;
static uint32_t current_checkpoint_index_debug;

/**
 * NON-STATIC FUNCTIONS
 */

void geo_logic__run_once() { current_coordinate = gps__get_coordinates(); }

void geo_logic__update_destination_coordinate(dbc_SENSOR_BT_COORDINATES_s *destination) {
  // destination_coordinate = *destination;
  destination_gps_coordinate.latitude = destination->SENSOR_BT_COORDINATES_latitude;
  destination_gps_coordinate.longitude = destination->SENSOR_BT_COORDINATES_longitude;

  current_checkpoint = geo_logic__compute_next_checkpoint();
}

float geo_logic__compute_required_bearing() {

  if (geo_logic__checkpoint_reached()) {
    // Recompute here and then continue
    current_checkpoint = geo_logic__compute_next_checkpoint();
  }

  /* Haversine Formula for getting required bearing angle */
  float delta_longitude = current_checkpoint.longitude - current_coordinate.longitude;

  float y = sinf(delta_longitude) * cosf(current_checkpoint.latitude);

  float x = cosf(current_coordinate.latitude) * sinf(current_checkpoint.latitude) -
            sinf(current_coordinate.latitude) * cosf(current_checkpoint.latitude) * cosf(delta_longitude);

  float bearing_angle = atan2f(y, x);
  bearing_angle = geo_logic__radian_to_degree(bearing_angle);

  /* Convert from -180 to 180 -> 0 to 360 */
  bearing_angle = ((uint32_t)(bearing_angle + 180)) % 360;

  return bearing_angle;
}

float geo_logic__distance_from_destination() {
  return geo_logic__distance_between_two_coordinates(&current_coordinate, &destination_gps_coordinate);
}

dbc_GEO_DESTINATION_REACHED_s geo_logic__compute_destination_reached() {
  dbc_GEO_DESTINATION_REACHED_s rval = {};
  rval.GEO_DESTINATION_REACHED_cmd = GEO_DESTINATION_REACHED_cmd_NOT_REACHED;

  if (geo_logic__distance_from_destination() < DESTINATION_REACHED_RADIUS_METERS) {
    rval.GEO_DESTINATION_REACHED_cmd = GEO_DESTINATION_REACHED_cmd_REACHED;
  }
  return rval;
}

// DEBUG
uint32_t geo_logic__get_current_checkpoint_index_debug() { return current_checkpoint_index_debug; }

/**
 * STATIC FUNCTIONS
 */
static gps_coordinates_s geo_logic__compute_next_checkpoint() {
  float max_distance = geo_logic__distance_from_destination();
  float min_distance = max_distance;
  gps_coordinates_s selected_checkpoint = destination_gps_coordinate;

  for (int i = 0; i < sizeof(CHECKPOINTS) / sizeof(gps_coordinates_s); i++) {
    gps_coordinates_s temporary_checkpoint = CHECKPOINTS[i];

    float source_to_destination_distance =
        geo_logic__distance_between_two_coordinates(&current_coordinate, &temporary_checkpoint);

    float checkpoint_to_destination_distance =
        geo_logic__distance_between_two_coordinates(&temporary_checkpoint, &destination_gps_coordinate);

    if (checkpoint_to_destination_distance < max_distance) {
      if (min_distance > source_to_destination_distance) {
        min_distance = source_to_destination_distance;
        selected_checkpoint = temporary_checkpoint;
        current_checkpoint_index_debug = i;
      }
    }
  }

  return selected_checkpoint;
}

static bool geo_logic__checkpoint_reached() {
  bool rval = false;
  if (geo_logic__distance_between_two_coordinates(&current_coordinate, &current_checkpoint) <
      DESTINATION_REACHED_RADIUS_METERS) {
    rval = true;
  }

  return rval;
}

static float geo_logic__distance_between_two_coordinates(const gps_coordinates_s *source_coordinates,
                                                         const gps_coordinates_s *destination_coordinates) {

  float current_coordinate_latitude_radian = geo_logic__degree_to_radian(source_coordinates->latitude);

  float destination_coordinate_latitude_radian = geo_logic__degree_to_radian(destination_coordinates->latitude);

  float delta_latitude = geo_logic__degree_to_radian(destination_coordinates->latitude - source_coordinates->latitude);

  float delta_longitude =
      geo_logic__degree_to_radian(destination_coordinates->longitude - source_coordinates->longitude);

  float a = sinf(delta_latitude / 2) * sinf(delta_latitude / 2) +
            cosf(current_coordinate_latitude_radian) * cosf(destination_coordinate_latitude_radian) *
                sinf(delta_longitude / 2) * sinf(delta_longitude / 2);

  float c = 2 * atan2f(sqrtf(a), sqrtf(1 - a));
  float d = EARTH_RADIUS * c;
  return d;
}

float geo_logic__degree_to_radian(float degree) {
  float rval = (degree * PI) / 180;
  return rval;
}

float geo_logic__radian_to_degree(float radian) {
  float rval = (radian * 180) / PI;
  return rval;
}
