#pragma once

#include "board_io.h"
#include "gpio.h"

void can_led__init(void);
bool can_led__switch3_pressed(void);

void can_led__led0_ON(void);
void can_led__led0_OFF(void);
void can_led__led1_ON(void);
void can_led__led1_OFF(void);
void can_led__led2_ON(void);
void can_led__led2_OFF(void);
void can_led__led3_ON(void);
void can_led__led3_OFF(void);
