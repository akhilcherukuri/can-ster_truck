#pragma once

#include <stdbool.h>

void motor__init();
void motor__run_10hz(int callback_count);
extern bool mia_steering;
