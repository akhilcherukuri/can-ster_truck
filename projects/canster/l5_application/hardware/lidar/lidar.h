#pragma once

#include "gpio.h"
#include "pwm1.h"

#include "lidar_data_handler.h"
#include "uart.h"

#include <stdint.h>
#include <string.h>

#define DO_NOT_RESET_EVERY_MINUTE 1

void lidar__config_init(void);

void lidar__stop(void);
void lidar__reset(void);
void lidar__sample_scan(void);
void lidar__scan_run_once(int send_once);
void lidar__express_scan(void);
void lidar__force_scan(void);

void lidar__get_info(void);
void lidar__get_health(void);
void lidar__get_samplerate(void);
void lidar__get_conf(void);

void lidar__receive_data_response_check(void);
