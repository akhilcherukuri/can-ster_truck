#pragma once

#include <stdint.h>
void servo__steer_init(void);
void servo__steer_processor(int16_t steering_value); // [-2,2]