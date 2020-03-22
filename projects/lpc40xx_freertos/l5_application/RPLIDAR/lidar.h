#pragma once

#include "gpio.h"
#include "pwm1.h"
#include "stdint.h"
#include "uart.h"

void lidar__config_init(void);
void lidar__sample_scan(void);
void lidar__express_scan(void);

void lidar__receive_data_response(void);
