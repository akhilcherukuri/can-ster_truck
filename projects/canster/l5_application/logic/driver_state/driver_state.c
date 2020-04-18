#include "driver_state.h"

static dbc_DRIVER_COORDINATES_s destination_coordinate;

void driver_state__update_destination_coordinate(dbc_DRIVER_COORDINATES_s *coordinates) {
  destination_coordinate = *coordinates;
}

// ! Do not discard the const qualifier
const dbc_DRIVER_COORDINATES_s *driver_state__get_destination_coordinate(void) { return &destination_coordinate; }