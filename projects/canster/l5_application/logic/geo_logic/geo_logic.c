#include "geo_logic.h"

#include "gps.h"
#include <math.h>

#include <stdio.h>

// Static functions
static float geo_logic__degree_to_radian(float degree);
static float geo_logic__radian_to_degree(float radian);

/**
 *
 */
static const float PI = 3.141592654;
static const uint32_t EARTH_RADIUS = 6371 * 1000;
static const uint32_t DESTINATION_REACHED_RADIUS = 1; // meter

/**
 * State Variables
 */
static dbc_DRIVER_COORDINATES_s destination_coordinate;

/**
 * FUNCTIONS
 */
void geo_logic__update_destination_coordinate(dbc_DRIVER_COORDINATES_s *destination) {
  destination_coordinate = *destination;
}

float geo_logic__compute_required_bearing() {
  // Get the data here
  gps_coordinates_s current_coordinate = gps__get_coordinates();

  // DONE, Compute the Haversine formula here
  float delta_longitude = destination_coordinate.DRIVER_COORDINATES_longitude - current_coordinate.longitude;

  float y = sinf(delta_longitude) * cosf(destination_coordinate.DRIVER_COORDINATES_latitude);

  float x = cosf(current_coordinate.latitude) * sinf(destination_coordinate.DRIVER_COORDINATES_latitude) -
            sinf(current_coordinate.latitude) * cosf(destination_coordinate.DRIVER_COORDINATES_latitude) *
                cosf(delta_longitude);

  float bearing_angle = atan2f(y, x);
  bearing_angle = geo_logic__radian_to_degree(bearing_angle);

  // Convert from -180 to 180 -> 0 to 360
  bearing_angle = ((uint32_t)(bearing_angle + 180)) % 360;

  // return the bearing angle
  return bearing_angle;
}

float geo_logic__distance_from_destination() {
  gps_coordinates_s current_coordinate = gps__get_coordinates();

  float current_coordinate_latitude_radian = geo_logic__degree_to_radian(current_coordinate.latitude);
  float destination_coordinate_latitude_radian =
      geo_logic__degree_to_radian(destination_coordinate.DRIVER_COORDINATES_latitude);

  float delta_latitude =
      geo_logic__degree_to_radian(destination_coordinate.DRIVER_COORDINATES_latitude - current_coordinate.latitude);
  float delta_longitude =
      geo_logic__degree_to_radian(destination_coordinate.DRIVER_COORDINATES_longitude - current_coordinate.longitude);

  float a = sinf(delta_latitude / 2) * sinf(delta_latitude / 2) +
            cosf(current_coordinate_latitude_radian) * cosf(destination_coordinate_latitude_radian) *
                sinf(delta_longitude / 2) * sinf(delta_longitude / 2);

  float c = 2 * atan2f(sqrtf(a), sqrtf(1 - a));
  float d = EARTH_RADIUS * c;
  return d;
}

dbc_GEO_DESTINATION_REACHED_s geo_logic__compute_destination_reached() {
  dbc_GEO_DESTINATION_REACHED_s rval = {};
  rval.GEO_DESTINATION_REACHED_cmd = GEO_DESTINATION_REACHED_cmd_NOT_REACHED;

  if (geo_logic__distance_from_destination() < DESTINATION_REACHED_RADIUS) {
    rval.GEO_DESTINATION_REACHED_cmd = GEO_DESTINATION_REACHED_cmd_REACHED;
  }

  return rval;
}

/**
 * STATIC DEFINITIONS
 */
static float geo_logic__degree_to_radian(float degree) {
  float rval = (degree * PI) / 180;
  return rval;
}

static float geo_logic__radian_to_degree(float radian) {
  float rval = (radian * 180) / PI;
  return rval;
}
