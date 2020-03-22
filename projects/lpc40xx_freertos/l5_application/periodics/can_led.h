#pragma once

#include "board_io.h"
#include "gpio.h"

void can_led__init(void);
bool can_led__switch_pressed(void);
void can_led__led3_ON(void);
void can_led__led3_OFF(void);
