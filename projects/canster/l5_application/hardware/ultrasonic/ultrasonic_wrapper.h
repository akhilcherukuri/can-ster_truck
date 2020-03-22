#pragma once

#include "ultrasonic.h"

typedef struct {
  float right, left, middle;
} ultrasonic_distance_s;

void ultrasonic__init_all_sensors(void);
void ultrasonic__update_all_sensors(void);

void ultrasonic__get_distance_from_all_sensors(ultrasonic_distance_s *output);
