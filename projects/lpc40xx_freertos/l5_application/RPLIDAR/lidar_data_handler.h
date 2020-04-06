#pragma once

#include "can_led.h"
#include <stdint.h>
#include <stdio.h>

void lidar_data_handler_init(void);
// static uint16_t min_distance(uint16_t *distance_data, uint8_t data_size);
// static void lidar_data_response_handle_distance(uint16_t angle, uint16_t distance, uint8_t quality);
void lidar_data_response_parse(uint8_t *data);
void check_range(void);
void within_range(void);
