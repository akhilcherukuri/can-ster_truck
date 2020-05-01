#pragma once

#include "lidar.h"
#include "lidar_data_handler.h"

void lidar__init(void);
void lidar__run_1hz(int callback_count);
void lidar__run_100Hz(int callback_count);
void lidar__run_1000Hz(void);
