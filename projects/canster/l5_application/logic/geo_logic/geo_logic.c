#include "geo_logic.h"

#include "gps.h"
#include <math.h>

#define PI 3.141592654

static dbc_DRIVER_COORDINATES_s destination_coordinate;

void geo_logic__update_destination_coordinate(dbc_DRIVER_COORDINATES_s *destination) {
  destination_coordinate = *destination;
}

float geo_logic__compute_required_bearing() {
  // Get the data here
  gps_coordinates_s current_coordinate = gps__get_coordinates();
  // destination_coordinate.DRIVER_COORDINATES_latitude
  // destination_coordinate.DRIVER_COORDINATES_longitude

  // DONE, Compute the Haversine formula here
  float delta_longitude = destination_coordinate.DRIVER_COORDINATES_longitude - current_coordinate.longitude;

  float y = sin(delta_longitude) * cos(destination_coordinate.DRIVER_COORDINATES_latitude);

  float x =
      cos(current_coordinate.latitude) * sin(destination_coordinate.DRIVER_COORDINATES_latitude) -
      sin(current_coordinate.latitude) * cos(destination_coordinate.DRIVER_COORDINATES_latitude) * cos(delta_longitude);

  float bearing_angle = atan2(y, x) * 180.0 / PI; // convert to degree value

  // return the bearing angle
  return bearing_angle;
}
