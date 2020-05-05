#include "unity.h"

#include "Mocklcd.h"
#include "Mocklcd_wrapper.h"

#include "Mockcan_geo_node.h"
#include "Mockcan_motor_node.h"

#include "project.h"

#include "lcd_ui.c"

void setUp() {}
void tearDown() {}

// ! Update the tests here as needed
void test_lcd_ui__update_sensor_values(void) {
  can_motor__get_motor_speed_feedback_ExpectAndReturn(current_speed_s);
  can_geo__get_current_coordinates_ExpectAndReturn(curr_coodinates);

  lcd_ui__update_sensor_values();
}

// ! Update the tests as the UI changes
void test_lcd_ui__update_ui(void) {
  bool UNUSED_RETURN_BOOL = true;

  lcd_wrapper__write_led_ExpectAndReturn(LCD_LED_DRIVER_MIA_INDEX, driver_mia_led, UNUSED_RETURN_BOOL);
  lcd_wrapper__write_led_ExpectAndReturn(LCD_LED_SENSOR_MIA_INDEX, sensor_mia_led, UNUSED_RETURN_BOOL);
  lcd_wrapper__write_led_ExpectAndReturn(LCD_LED_GEO_MIA_INDEX, geo_mia_led, UNUSED_RETURN_BOOL);
  lcd_wrapper__write_led_ExpectAndReturn(LCD_LED_MOTOR_MIA_INDEX, motor_mia_led, UNUSED_RETURN_BOOL);

  // kph is always in the range of 0 - 10
  lcd_wrapper__write_meter_ExpectAndReturn(LCD_METER_KPH_INDEX, kph_meter % 11, UNUSED_RETURN_BOOL);

  // These will be in the Range of 0-99
  lcd_wrapper__write_leddigit_ExpectAndReturn(LCD_LEDDIGIT_rps_INDEX, rps_leddigit, UNUSED_RETURN_BOOL);
  lcd_wrapper__write_leddigit_ExpectAndReturn(LCD_LEDDIGIT_CURRENTDEGREE_INDEX, cdegree_leddigit, UNUSED_RETURN_BOOL);
  lcd_wrapper__write_leddigit_ExpectAndReturn(LCD_LEDDIGIT_REQUIREDDEGREE_INDEX, rdegree_leddigit, UNUSED_RETURN_BOOL);
  lcd_wrapper__write_leddigit_ExpectAndReturn(LCD_LEDDIGIT_CURR_LAT_INDEX, curr_lat_leddigit, UNUSED_RETURN_BOOL);
  lcd_wrapper__write_leddigit_ExpectAndReturn(LCD_LEDDIGIT_CURR_LONG_INDEX, curr_long_leddigit, UNUSED_RETURN_BOOL);

  lcd_wrapper__write_leddigit_ExpectAndReturn(LCD_LIDAR_LEFT_SECTOR_DISTANCE_INDEX, lidar_value.left_sector_distance,
                                              UNUSED_RETURN_BOOL);
  lcd_wrapper__write_leddigit_ExpectAndReturn(LCD_LIDAR_MIDDLE_SECTOR_DISTANCE_INDEX,
                                              lidar_value.middle_sector_distance, UNUSED_RETURN_BOOL);
  lcd_wrapper__write_leddigit_ExpectAndReturn(LCD_LIDAR_RIGHT_SECTOR_DISTANCE_INDEX, lidar_value.right_sector_distance,
                                              UNUSED_RETURN_BOOL);

  lcd_ui__update_ui();
}

// FUNCTIONS

void test_lcd_ui__init(void) {
  lcd__init_Expect();
  lcd__reset_Expect();

  lcd_ui__init();
}
