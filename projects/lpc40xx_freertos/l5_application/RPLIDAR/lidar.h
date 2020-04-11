#pragma once

#include "gpio.h"
#include "pwm1.h"

#include "can_led.h"
#include "lidar_data_handler.h"
#include "uart.h"
#include <string.h>

#include <stdint.h>

void lidar__config_init(void);

void lidar__stop(void);
void lidar__reset(void);
void lidar__sample_scan(void);
void lidar__sample_scan_run_once(int send_once);
void lidar__express_scan(void);
void lidar__force_scan(void);

void lidar__get_info(void);
void lidar__get_health(void);
void lidar__get_samplerate(void);
void lidar__get_conf(void);

// void lidar__receive_data_response(void);
void lidar__receive_data_response_check(void);
