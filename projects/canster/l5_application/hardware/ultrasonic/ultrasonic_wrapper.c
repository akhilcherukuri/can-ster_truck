#include "ultrasonic_wrapper.h"

#include "gpio.h"
#include "ultrasonic.h"

/**
 * ULTRASONIC SENSOR PORT-PIN MAPPING SUMMARY:
 * SENSOR_LEFT - Echo: 0.6, Trigger: 0.7
 * SENSOR_MIDDLE - Echo: 0.8, Trigger: 0.9
 * SENSOR_RIGHT - Echo: 0.26, Trigger: 0.25
 */

/**
 * STATIC CONST VARIABLES
 */
static const uint8_t left_sensor_echo_input_pin = 6, left_sensor_trig_output_pin = 7;
static const uint8_t middle_sensor_echo_input_pin = 8, middle_sensor_trig_output_pin = 9;
static const uint8_t right_sensor_echo_input_pin = 26, right_sensor_trig_output_pin = 25;
static const gpio__port_e left_sensor_echo_input_port = GPIO__PORT_0, left_sensor_trig_output_port = GPIO__PORT_0;
static const gpio__port_e middle_sensor_echo_input_port = GPIO__PORT_0, middle_sensor_trig_output_port = GPIO__PORT_0;
static const gpio__port_e right_sensor_echo_input_port = GPIO__PORT_0, right_sensor_trig_output_port = GPIO__PORT_0;

/**
 * STATIC FUNCTIONS
 */
static void ultrasonic__init_ports_and_pins_left(void);
static void ultrasonic__init_ports_and_pins_middle(void);
static void ultrasonic__init_ports_and_pins_right(void);

/**
 * STATIC VARIABLES
 */
static ultrasonic_sensor_s sensor_left, sensor_middle, sensor_right;

/**
 * NON-STATIC FUNCTION DEFINITIONS
 */
void ultrasonic__init_all_sensors(void) {
  ultrasonic__init(&sensor_left);
  ultrasonic__init(&sensor_middle);
  ultrasonic__init(&sensor_right);
}

void ultrasonic__init_ports_and_pins(void) {
  ultrasonic__init_ports_and_pins_left();
  ultrasonic__init_ports_and_pins_middle();
  ultrasonic__init_ports_and_pins_right();
}

ultrasonic_sensor_s *get__sensor_left() { return &sensor_left; }
ultrasonic_sensor_s *get__sensor_middle() { return &sensor_middle; }
ultrasonic_sensor_s *get__sensor_right() { return &sensor_right; }

const double ultrasonic__get_obstacle_distance(ultrasonic_sensor_s *sensor_val) {
  ultrasonic__calculate_distance_from_obstacle(sensor_val);
  return sensor_val->distance_from_obstacle;
}

/**
 * STATIC FUNCTION DEFINITIONS
 */
static void ultrasonic__init_ports_and_pins_left() {
  sensor_left.echo_input.port_number = left_sensor_echo_input_port;
  sensor_left.echo_input.pin_number = left_sensor_echo_input_pin;
  sensor_left.trig_output.port_number = left_sensor_trig_output_port;
  sensor_left.trig_output.pin_number = left_sensor_trig_output_pin;
}

static void ultrasonic__init_ports_and_pins_middle() {
  sensor_middle.echo_input.port_number = middle_sensor_echo_input_port;
  sensor_middle.echo_input.pin_number = middle_sensor_echo_input_pin;
  sensor_middle.trig_output.port_number = middle_sensor_trig_output_port;
  sensor_middle.trig_output.pin_number = middle_sensor_trig_output_pin;
}

static void ultrasonic__init_ports_and_pins_right() {
  sensor_right.echo_input.port_number = right_sensor_echo_input_port;
  sensor_right.echo_input.pin_number = right_sensor_echo_input_pin;
  sensor_right.trig_output.port_number = right_sensor_trig_output_port;
  sensor_right.trig_output.pin_number = right_sensor_trig_output_pin;
}

/**
 * USAGE:
 * 1. Call ultrasonic__init_ports_and_pins(), ultrasonic__init_all_sensors() in periodic_callbacks__initialize()
 * 2. double distance = ultrasonic__get_obstacle_distance(get__sensor_left()); // Call in driver_obstacle_detection() logic
 */