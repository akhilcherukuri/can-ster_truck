#include "lcd_wrapper.h"

#include "lcd.h"

/**
 * STATIC FUNCTION DECLARATION
 */
bool lcd_wrapper__verify_write_response(uint8_t response);
bool lcd_wrapper__verify_read_response(uint8_t response);

/**
 * FUNCTIONS
 */

/**
 * READ FUNCTIONS
 */
bool lcd_wrapper__read_led(uint8_t index) {
  uint8_t response;
  uint16_t response_data;
  lcd__read(LCD_USERLED, index, &response, &response_data);

  if (!lcd_wrapper__verify_read_response(response)) {
    response_data = 0;
  }

  // Return true of false depending on the response_data of screen
  bool rval = false;
  if (response_data) {
    rval = true;
  }

  return rval;
}

uint16_t lcd_wrapper__read_meter(uint8_t index) {
  uint8_t response;
  uint16_t response_data;
  lcd__read(LCD_METER, index, &response, &response_data);

  if (!lcd_wrapper__verify_read_response(response)) {
    response_data = 0;
  }

  return response_data;
}

uint16_t lcd_wrapper__read_leddigit(uint8_t index) {
  uint8_t response;
  uint16_t response_data;
  lcd__read(LCD_LEDDIGIT, index, &response, &response_data);

  if (!lcd_wrapper__verify_read_response(response)) {
    response_data = 0;
  }

  return response_data;
}

/**
 * WRITE FUNCTIONS
 */

bool lcd_wrapper__write_led(uint8_t index, bool value) {
  uint8_t response;
  lcd__write(LCD_USERLED, index, 0x00, (uint8_t)value, &response);

  return lcd_wrapper__verify_write_response(response);
}

bool lcd_wrapper__write_meter(uint8_t index, uint16_t value) {
  uint8_t response;
  lcd__write(LCD_METER, index, (value >> 8) & 0xff, value & 0xff, &response);

  return lcd_wrapper__verify_write_response(response);
}

bool lcd_wrapper__write_leddigit(uint8_t index, uint16_t value) {
  uint8_t response;
  lcd__write(LCD_LEDDIGIT, index, (value >> 8) & 0xff, value & 0xff, &response);

  // DONE, Verify the value received here
  return lcd_wrapper__verify_write_response(response);
}

/**
 * STATIC FUNCTIONS
 */
bool lcd_wrapper__verify_write_response(uint8_t response) {
  bool rval = false;
  if (response == LCD_ACK) {
    rval = true;
  }

  return rval;
}

bool lcd_wrapper__verify_read_response(uint8_t response) {
  bool rval = false;
  if (response == LCD_REPORT_OBJECT) {
    rval = true;
  }

  return rval;
}
