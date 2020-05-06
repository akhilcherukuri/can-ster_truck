/**
 * Haversine formula
 * https://en.wikipedia.org/wiki/Haversine_formula
 * https://www.movable-type.co.uk/scripts/latlong.html
 *
 * https://www.vcalc.com/wiki/vCalc/Haversine+-+Distance
 *
 * Convert 2 points on the map to bearing degree
 * NOTE: Compass 0 degree always points to true north (North Pole)
 */
#pragma once

#include "project.h"

void geo_logic__update_destination_coordinate(dbc_SENSOR_BT_COORDINATES_s *destination);

float geo_logic__compute_required_bearing();

/**
 * @brief Returns distance from destination in meters
 *
 * @return float
 */
float geo_logic__distance_from_destination();

/**
 * @brief Check if you are within a certain radius
 *
 * @return dbc_GEO_DESTINATION_REACHED_s
 */
dbc_GEO_DESTINATION_REACHED_s geo_logic__compute_destination_reached();
