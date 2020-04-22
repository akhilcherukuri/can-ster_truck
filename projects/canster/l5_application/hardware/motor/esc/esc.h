#pragma once

#include <stdint.h>
#include <stdio.h>

void esc__init();

void esc__forward_slow();
void esc__forward_medium();
void esc__forward_fast();
void esc__neutral();
void esc__reverse_slow();
void esc__reverse_medium();
void esc__reverse_fast();

void esc__direction_processor(int16_t direction_value);

void esc__calibrate_blink_red_once();
void esc__calibrate_blink_red_twice();
void esc__calibrate_red_to_green();