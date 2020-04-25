/**
 * Haversine formula
 * https://en.wikipedia.org/wiki/Haversine_formula
 * https://www.movable-type.co.uk/scripts/latlong.html
 *
 * Convert 2 points on the map to bearing degree
 * NOTE: Compass 0 degree always points to true north (North Pole)
 */
#pragma once

#include "project.h"

void geo_logic__update_destination_coordinate(dbc_SENSOR_BT_COORDINATES_s *destination);

float geo_logic__compute_required_bearing();