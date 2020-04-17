/**
 * @file lcd.h
 *
 * Low Level uLCD Driver - uLCD PTU
 *
 * ViSi-Genie Reference Manual
 * Page 37: Object Summary Table
 * Page 6: Protocol Definition
 */
#pragma once

#include <stdint.h>

/**
 * @brief Add lcd object ids here
 */
typedef enum {
  LCD_LEDDIGIT = 0x0F,
  LCD_METER = 0x10,
  LCD_USERLED = 0x13,
} lcd_object_e;

/**
 * @brief The different identifiers can be mapped to special functions if needed
 */
typedef enum {
  LCD_REPORT_OBJECT = 0x05,
  LCD_ACK = 0x06,
  LCD_NACK = 0x15,
} lcd_response_id_e;

void lcd__init(void);
void lcd__reset(void);

void lcd__write(lcd_object_e object_id, uint8_t object_index, uint8_t msb, uint8_t lsb, uint8_t *response);
void lcd__read(lcd_object_e object_id, uint8_t object_index, uint8_t *response, uint16_t *response_data);
