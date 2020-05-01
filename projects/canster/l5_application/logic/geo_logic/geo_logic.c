#include "geo_logic.h"

#include "gps.h"
#include <math.h>

#include <stdio.h>

/**
 * STATIC FUNCTIONS
 */
static float geo_logic__degree_to_radian(float degree);
static float geo_logic__radian_to_degree(float radian);

/**
 * STATIC CONST VARIABLES
 */
static const float PI = 3.141592654;
static const uint32_t EARTH_RADIUS = 6371 * 1000;
static const float DESTINATION_REACHED_RADIUS_METERS = 4.0;

/**
 * STATE VARIABLES
 */
// static dbc_DRIVER_COORDINATES_s destination_coordinate = {{0}, 37.340679, -121.894371}; // Uncomment for testing
static dbc_DRIVER_COORDINATES_s destination_coordinate;

/**
 * NON-STATIC FUNCTIONS
 */
static dbc_SENSOR_BT_COORDINATES_s destination_coordinate;

void geo_logic__update_destination_coordinate(dbc_SENSOR_BT_COORDINATES_s *destination) {
  destination_coordinate = *destination;
}

float geo_logic__compute_required_bearing() {
  gps_coordinates_s current_coordinate = gps__get_coordinates();

  /* Haversine Formula for getting required bearing angle */
  float delta_longitude = destination_coordinate.DRIVER_COORDINATES_longitude - current_coordinate.longitude;

  float y = sinf(delta_longitude) * cosf(destination_coordinate.DRIVER_COORDINATES_latitude);

  float x = cosf(current_coordinate.latitude) * sinf(destination_coordinate.DRIVER_COORDINATES_latitude) -
            sinf(current_coordinate.latitude) * cosf(destination_coordinate.DRIVER_COORDINATES_latitude) *
                cosf(delta_longitude);

  float bearing_angle = atan2f(y, x);
  bearing_angle = geo_logic__radian_to_degree(bearing_angle);

  /* Convert from -180 to 180 -> 0 to 360 */
  bearing_angle = ((uint32_t)(bearing_angle + 180)) % 360;

  return bearing_angle;
}

float geo_logic__distance_from_destination() {
  gps_coordinates_s current_coordinate = gps__get_coordinates();
  printf("\nCurrent coordinates from GPS = Lat: %lf Long: %lf", (double)current_coordinate.latitude,
         (double)current_coordinate.longitude);
  printf("\nDestination coordinates from Sensor Node (Bluetooth app) = Lat: %lf Long: %lf",
         (double)destination_coordinate.DRIVER_COORDINATES_latitude,
         (double)destination_coordinate.DRIVER_COORDINATES_longitude);

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
  printf("\nDistance between current coordinate and destination: %lf", (double)d);
  return d;
}

dbc_GEO_DESTINATION_REACHED_s geo_logic__compute_destination_reached() {
  dbc_GEO_DESTINATION_REACHED_s rval = {};
  rval.GEO_DESTINATION_REACHED_cmd = GEO_DESTINATION_REACHED_cmd_NOT_REACHED;

  if (geo_logic__distance_from_destination() < DESTINATION_REACHED_RADIUS_METERS) {
    rval.GEO_DESTINATION_REACHED_cmd = GEO_DESTINATION_REACHED_cmd_REACHED;
  }
  return rval;
}

/**
 * STATIC FUNCTIONS
 */
static float geo_logic__degree_to_radian(float degree) {
  float rval = (degree * PI) / 180;
  return rval;
}

static float geo_logic__radian_to_degree(float radian) {
  float rval = (radian * 180) / PI;
  return rval;
}
