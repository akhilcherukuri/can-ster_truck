#pragma once

#include "project.h"

void driver_state__update_destination_coordinate(dbc_DRIVER_COORDINATES_s *coordinates);

// ! Do not discard the const qualifier
const dbc_DRIVER_COORDINATES_s *driver_state__get_destination_coordinate(void);
