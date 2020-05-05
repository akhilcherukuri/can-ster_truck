#include "lcd_ui.h"

#include "lcd.h"
#include "lcd_wrapper.h"

#include <stdint.h>

#include "project.h"

#include "can_motor_node.h"

/**
 * UI Indexes here
 */
static const uint8_t LCD_LED_DRIVER_MIA_INDEX = 0x00;
static const uint8_t LCD_LED_SENSOR_MIA_INDEX = 0x01;
static const uint8_t LCD_LED_GEO_MIA_INDEX = 0x02;
static const uint8_t LCD_LED_MOTOR_MIA_INDEX = 0x03;

static const uint8_t LCD_METER_KPH_INDEX = 0x00;

static const uint8_t LCD_LEDDIGIT_RPM_INDEX = 0x00;
static const uint8_t LCD_LEDDIGIT_CURRENTDEGREE_INDEX = 0x01;
static const uint8_t LCD_LEDDIGIT_REQUIREDDEGREE_INDEX = 0x02;

// TODO, Update more here

/**
 * STATE Variables here
 */
static bool driver_mia_led;
static bool sensor_mia_led;
static bool geo_mia_led;
static bool motor_mia_led;

static uint16_t kph_meter;
dbc_MOTOR_SPEED_FEEDBACK_s current_speed_s;

static uint16_t rpm_leddigit;
static uint16_t cdegree_leddigit;
static uint16_t rdegree_leddigit;

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

/**
 * STATIC FUNCTION DEFINITIONS
 */
// TODO, Update these values from the decode functions
static void lcd_ui__update_sensor_values(void) {
  current_speed_s = can_motor__get_motor_speed_feedback();
  kph_meter = (uint16_t)current_speed_s.MOTOR_SPEED_current;
}

static void lcd_ui__update_ui(void) {

  lcd_wrapper__write_led(LCD_LED_DRIVER_MIA_INDEX, driver_mia_led);
  lcd_wrapper__write_led(LCD_LED_SENSOR_MIA_INDEX, sensor_mia_led);
  lcd_wrapper__write_led(LCD_LED_GEO_MIA_INDEX, geo_mia_led);
  lcd_wrapper__write_led(LCD_LED_MOTOR_MIA_INDEX, motor_mia_led);

  // kph is always in the range of 0 - 10
  lcd_wrapper__write_meter(LCD_METER_KPH_INDEX, kph_meter % 11);

  // These will be in the Range of 0-99
  lcd_wrapper__write_leddigit(LCD_LEDDIGIT_RPM_INDEX, rpm_leddigit % 100);
  lcd_wrapper__write_leddigit(LCD_LEDDIGIT_CURRENTDEGREE_INDEX, cdegree_leddigit % 100);
  lcd_wrapper__write_leddigit(LCD_LEDDIGIT_REQUIREDDEGREE_INDEX, rdegree_leddigit % 100);
}
