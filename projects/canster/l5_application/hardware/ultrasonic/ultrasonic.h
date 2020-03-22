/**
 * Datasheet -> https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf
 */
#pragma once

#include <stdint.h>

#include "gpio.h"

#define ULTRASONIC_DEBUG 0
#define ULTRASONIC_ISR_DEBUG 0
#define MAX_ULTRASONIC_LENGTH 3 // Number of Physical Ultrasonic Sensors in use

typedef struct {
  uint64_t clock_time_at_rising_edge, clock_time_at_falling_edge;
  gpio_s echo_input, trig_output;
  double distance_from_obstacle;
} ultrasonic_sensor_s;

void ultrasonic__init(ultrasonic_sensor_s *u_sensor);
void ultrasonic__calculate_distance_from_obstacle(ultrasonic_sensor_s *u_sensor);