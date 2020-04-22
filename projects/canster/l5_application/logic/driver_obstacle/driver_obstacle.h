#pragma once

#include "project.h"

void driver_obstacle__process_input(dbc_SENSOR_SONARS_s *sonar);

dbc_MOTOR_STEERING_s driver_obstacle__get_motor_commands();
int16_t driver_obstacle__move_to_destination();

void driver_obstacle__set_geo_controller_direction(dbc_GEO_DEGREE_s *);

float driver_obstacle__get_motor_speed_value();