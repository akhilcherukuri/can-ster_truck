#include "geo_logic.h"

#include "gps.h"
#include <math.h>

static dbc_DRIVER_COORDINATES_s destination_coordinate;

void geo_logic__update_destination_coordinate(dbc_DRIVER_COORDINATES_s *destination) {
  destination_coordinate = *destination;
}

float geo_logic__compute_required_bearing() {
  float rval = 0;

  // Get the data here
  gps_coordinates_s current_coordinate = gps__get_coordinates();
  // destination_coordinate.DRIVER_COORDINATES_latitude
  // destination_coordinate.DRIVER_COORDINATES_longitude

  // TODO, Compute the Haversine formula here

  // return the bearing angle
  return rval;
}
