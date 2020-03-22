#pragma once

#include "ultrasonic.h"

void ultrasonic__init_all_sensors(void);
void ultrasonic__init_ports_and_pins(void);

ultrasonic_sensor_s *get__sensor_left(void);
ultrasonic_sensor_s *get__sensor_middle(void);
ultrasonic_sensor_s *get__sensor_right(void);

const double ultrasonic__get_obstacle_distance(ultrasonic_sensor_s *);