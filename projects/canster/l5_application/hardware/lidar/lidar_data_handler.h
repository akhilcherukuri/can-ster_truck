#pragma once

// #define DEBUG 1

// #if DEBUG
#include "can_led.h"
// #endif
#include "project.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

void lidar_data_handler__init(void);
void lidar_data_handler__response_parse(uint8_t *data);
void lidar_data_handler__retrieve_distance(void);
// void within_range(void);

bool lidar_data_handler__receive_five_byte_sample(char data);
void lidar_data_handler__get_distances(dbc_SENSOR_LIDAR_s *distances);
