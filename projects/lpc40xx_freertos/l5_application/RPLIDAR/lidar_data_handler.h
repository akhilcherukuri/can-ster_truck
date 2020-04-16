#pragma once

#include "can_led.h"
#include <stdint.h>
#include <stdio.h>

void lidar_data_handler_init(void);
void lidar_data_response_parse(uint8_t *data);
void check_range(void);
void within_range(void);

bool receive_five_byte_sample(char data);
void lidar_data_response_parse_v2(void);