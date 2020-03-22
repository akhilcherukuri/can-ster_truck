#pragma once

#include "gpio.h"
#include "pwm1.h"
#include "stdint.h"
#include "uart.h"

void lidar__config_init(void);
void lidar__reset(void);
void lidar__get_info(void);
void lidar__get_health(void);
void lidar__sample_scan(void);
void lidar__express_scan(void);

void lidar__receive_data_response(void);
