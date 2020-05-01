#pragma once

#include "project.h"

void driver_obstacle__process_input(dbc_SENSOR_SONARS_s *sonar);
void driver_obstacle__geo_controller_directions(dbc_GEO_DEGREE_s *gps_degree);

dbc_DRIVER_STEERING_s driver_obstacle__get_motor_commands();
