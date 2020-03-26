#pragma once

#include "delay.h"
#include "gpio.h"
#include "pwm1.h"
#include <stdio.h>

void servo__steer_init();

void servo__steer_processor(int steering_value); // [-2,2]