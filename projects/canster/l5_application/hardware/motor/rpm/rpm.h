#pragma once

#include <stdint.h>

void rpm__init();
float rpm__calculate_speed_kph();

float get_speed_kph();
uint16_t get_speed_rps();