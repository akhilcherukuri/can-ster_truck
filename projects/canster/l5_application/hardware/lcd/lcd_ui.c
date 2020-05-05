#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "lcd_ui.h"

#include "project.h"

#include "can_geo_node.h"
#include "can_motor_node.h"
#include "lcd.h"
#include "lcd_wrapper.h"

/* STATIC CONST VARIABLES */

/**
 * LCD cannot receive float/double datatypes. Hence we convert
 * them into uint16_t by multiplying PRECISION_FACTOR as per
 * requirement. Decimals are then automatically extracted by the
 * LCD hardware.
 */
static const double PRECISION_FACTOR = 10.0;
static const double CIRCUMFERENCE_METER = 0.314;
static const double MPS_TO_KPH_CONVERSION_FACTOR = 3.6;

/**
 * UI Indices here
 */
static const uint8_t LCD_LED_DRIVER_MIA_INDEX = 0x00;
static const uint8_t LCD_LED_SENSOR_MIA_INDEX = 0x01;
static const uint8_t LCD_LED_GEO_MIA_INDEX = 0x02;
static const uint8_t LCD_LED_MOTOR_MIA_INDEX = 0x03;

static const uint8_t LCD_METER_KPH_INDEX = 0x00;

static const uint8_t LCD_LEDDIGIT_rps_INDEX = 0x00;
static const uint8_t LCD_LEDDIGIT_CURRENTDEGREE_INDEX = 0x01;
static const uint8_t LCD_LEDDIGIT_REQUIREDDEGREE_INDEX = 0x02;
static const uint8_t LCD_LEDDIGIT_CURR_LAT_INDEX = 0x03;
static const uint8_t LCD_LEDDIGIT_CURR_LONG_INDEX = 0x04;
static const uint8_t LCD_LIDAR_LEFT_SECTOR_DISTANCE_INDEX = 0x05;
static const uint8_t LCD_LIDAR_MIDDLE_SECTOR_DISTANCE_INDEX = 0x06;
static const uint8_t LCD_LIDAR_RIGHT_SECTOR_DISTANCE_INDEX = 0x07;
// TODO, Update more here

/**
 * STATE Variables here
 */
static bool driver_mia_led;
static bool sensor_mia_led;
static bool geo_mia_led;
static bool motor_mia_led;

static uint16_t kph_meter;
static dbc_MOTOR_SPEED_FEEDBACK_s current_speed_s;
static dbc_GEO_CURRENT_COORDINATES_s curr_coodinates;
static lidar_distance_values_t lidar_value;

static uint16_t rps_leddigit;
static uint16_t cdegree_leddigit;
static uint16_t rdegree_leddigit;
static uint16_t curr_lat_leddigit;
static uint16_t curr_long_leddigit;
// TODO, Update more here

/**
 * Static function declarations
 */
static void lcd_ui__update_sensor_values(void);
static void lcd_ui__update_ui(void);

/**
 * FUNCTIONS
 */
void lcd_ui__init(void) {
  lcd__init();
  lcd__reset(); // Reset the lcd on every init
}

void lcd_ui__run_once_1hz(void) {
  lcd_ui__update_sensor_values();
  lcd_ui__update_ui();
}

void set_lcd_rps() {
  double temp_calc = CIRCUMFERENCE_METER * MPS_TO_KPH_CONVERSION_FACTOR;
  rps_leddigit = (uint16_t)(((double)current_speed_s.MOTOR_SPEED_current / temp_calc) * PRECISION_FACTOR);
}

void set_lcd_current_and_required_heading(float curr_heading, float required_heading) {
  cdegree_leddigit = (uint16_t)((double)curr_heading * PRECISION_FACTOR);
  rdegree_leddigit = (uint16_t)((double)required_heading * PRECISION_FACTOR);
}

void set_lcd_mia_led(LCD_MIA_LED_e node_type, bool value) {
  switch (node_type) {
  case DRIVER_LED_MIA:
    driver_mia_led = value;
    break;
  case GEO_LED_MIA:
    geo_mia_led = value;
    break;
  case MOTOR_LED_MIA:
    motor_mia_led = value;
    break;
  case SENSOR_LED_MIA:
    sensor_mia_led = value;
    break;
  }
}

void set_lcd_current_coordinate_led(float curr_lat, float curr_long) {
  curr_lat_leddigit = (uint16_t)((double)curr_lat * PRECISION_FACTOR * PRECISION_FACTOR);
  curr_long_leddigit = (uint16_t)((double)curr_long * PRECISION_FACTOR * PRECISION_FACTOR);
}

void set_lcd_lidar_distance_led(dbc_SENSOR_LIDAR_s lidar_obstacle_distance) {
  lidar_value.left_sector_distance =
      (uint16_t)((double)(lidar_obstacle_distance.SENSOR_LIDAR_slight_left * PRECISION_FACTOR));
  lidar_value.middle_sector_distance =
      (uint16_t)((double)(lidar_obstacle_distance.SENSOR_LIDAR_middle * PRECISION_FACTOR));
  lidar_value.right_sector_distance =
      (uint16_t)((double)(lidar_obstacle_distance.SENSOR_LIDAR_slight_right * PRECISION_FACTOR));
}

/**
 * STATIC FUNCTION DEFINITIONS
 */
// TODO, Update these values from the decode functions
static void lcd_ui__update_sensor_values(void) {
  current_speed_s = can_motor__get_motor_speed_feedback();
  kph_meter = (uint16_t)current_speed_s.MOTOR_SPEED_current;
  set_lcd_rps();
  curr_coodinates = can_geo__get_current_coordinates();
  if (curr_coodinates.GEO_CURRENT_COORDINATES_latitude < 0) {
    curr_coodinates.GEO_CURRENT_COORDINATES_latitude *= (float)-1.0;
  }
  if (curr_coodinates.GEO_CURRENT_COORDINATES_longitude < 0) {
    curr_coodinates.GEO_CURRENT_COORDINATES_longitude *= (float)-1.0;
  }
  curr_lat_leddigit =
      (uint16_t)((double)curr_coodinates.GEO_CURRENT_COORDINATES_latitude * PRECISION_FACTOR * PRECISION_FACTOR);
  curr_long_leddigit =
      (uint16_t)((double)curr_coodinates.GEO_CURRENT_COORDINATES_longitude * PRECISION_FACTOR * PRECISION_FACTOR);
}

static void lcd_ui__update_ui(void) {
  lcd_wrapper__write_led(LCD_LED_DRIVER_MIA_INDEX, driver_mia_led);
  lcd_wrapper__write_led(LCD_LED_SENSOR_MIA_INDEX, sensor_mia_led);
  lcd_wrapper__write_led(LCD_LED_GEO_MIA_INDEX, geo_mia_led);
  lcd_wrapper__write_led(LCD_LED_MOTOR_MIA_INDEX, motor_mia_led);

  // kph is always in the range of 0 - 10
  lcd_wrapper__write_meter(LCD_METER_KPH_INDEX, kph_meter % 11);

  // These will be in the Range of 0 - 99
  lcd_wrapper__write_leddigit(LCD_LEDDIGIT_rps_INDEX, rps_leddigit);
  lcd_wrapper__write_leddigit(LCD_LEDDIGIT_CURRENTDEGREE_INDEX, cdegree_leddigit);
  lcd_wrapper__write_leddigit(LCD_LEDDIGIT_REQUIREDDEGREE_INDEX, rdegree_leddigit);
  lcd_wrapper__write_leddigit(LCD_LEDDIGIT_CURR_LAT_INDEX, curr_lat_leddigit);
  lcd_wrapper__write_leddigit(LCD_LEDDIGIT_CURR_LONG_INDEX, curr_long_leddigit);
  lcd_wrapper__write_leddigit(LCD_LIDAR_LEFT_SECTOR_DISTANCE_INDEX, lidar_value.left_sector_distance);
  lcd_wrapper__write_leddigit(LCD_LIDAR_MIDDLE_SECTOR_DISTANCE_INDEX, lidar_value.middle_sector_distance);
  lcd_wrapper__write_leddigit(LCD_LIDAR_RIGHT_SECTOR_DISTANCE_INDEX, lidar_value.right_sector_distance);
}
