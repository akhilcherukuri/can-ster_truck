#pragma once

#include "gps.h"

void bt_wrapper__read_once(void);
void bt_wrapper__write_once(void);

void bt_wrapper__get_destination_coordinates(gps_coordinates_s *dest_coordinate);
