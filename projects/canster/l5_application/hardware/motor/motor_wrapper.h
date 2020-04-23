#pragma once

#include <stdbool.h>
#include <stdint.h>

void motor__init();
void motor__run_10hz(uint32_t callback_count);