#pragma once

#include "gps.h"
#include "project.h"

void bt_wrapper__read_once(void);
void bt_wrapper__write_once(void);

void bt_wrapper__get_destination_coordinates(gps_coordinates_s *dest_coordinate);
void bt_wrapper__get_motor_key(dbc_MOTOR_KEY_s *motor_key);
