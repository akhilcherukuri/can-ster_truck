#pragma once

#include <stdbool.h>
#include <stdint.h>

bool lcd_wrapper__read_led(uint8_t index);
uint16_t lcd_wrapper__read_meter(uint8_t index);
uint16_t lcd_wrapper__read_leddigit(uint8_t index);

bool lcd_wrapper__write_led(uint8_t index, bool value);
bool lcd_wrapper__write_meter(uint8_t index, uint16_t value);
bool lcd_wrapper__write_leddigit(uint8_t index, uint16_t value);
