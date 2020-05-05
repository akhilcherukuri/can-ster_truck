#pragma once

#include <stdbool.h>

#include "project.h"

void lcd_ui__init(void);

typedef enum { DRIVER_LED_MIA, GEO_LED_MIA, MOTOR_LED_MIA, SENSOR_LED_MIA } LCD_MIA_LED_e;

typedef struct {
  uint16_t left_sector_distance;
  uint16_t middle_sector_distance;
  uint16_t right_sector_distance;
} lidar_distance_values_t;

// ! IMP run this in the 1hz function only
// * Reason: Contains 20-30ms potential delays in the low-level lcd module, when reading ack/nack/object response from
// lcd screen
void lcd_ui__run_once_1hz(void);

void set_lcd_rps();
void set_lcd_current_and_required_heading(float, float);
void set_lcd_mia_led(LCD_MIA_LED_e, bool);
void set_lcd_current_coordinate_led(float, float);
void set_lcd_lidar_distance_led(dbc_SENSOR_LIDAR_s);