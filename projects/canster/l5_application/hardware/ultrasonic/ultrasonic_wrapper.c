#include "ultrasonic_wrapper.h"

#include "gpio.h"

#include <stdio.h>

/**
 * ULTRASONIC SENSOR PORT-PIN MAPPING SUMMARY:
 * SENSOR_LEFT - Echo: 0.6, Trigger: 0.7
 * SENSOR_MIDDLE - Echo: 0.8, Trigger: 0.9
 * SENSOR_RIGHT - Echo: 0.26, Trigger: 0.25
 *
 * USAGE:
 * 1. Call ultrasonic__init_ports_and_pins(), ultrasonic__init_all_sensors() in periodic_callbacks__initialize()
 * 2. double distance = ultrasonic__get_obstacle_distance(get__sensor_left()); // Call in driver_obstacle_detection()
 * logic
 **/

/**
 * STATIC CONST VARIABLES
 */
// PINS
static const uint8_t ULTRASONIC_LEFT_ECHO_INPUT_PIN = 7, ULTRASONIC_LEFT_TRIG_OUTPUT_PIN = 6;

static const uint8_t ULTRASONIC_MIDDLE_ECHO_INPUT_PIN = 9, ULTRASONIC_MIDDLE_TRIG_OUTPUT_PIN = 8;

static const uint8_t ULTRASONIC_RIGHT_ECHO_INPUT_PIN = 25, ULTRASONIC_RIGHT_TRIG_OUTPUT_PIN = 26;

// PORTS
static const gpio__port_e COMMON_PORT = GPIO__PORT_0;

static const gpio__port_e ULTRASONIC_LEFT_SENSOR_INPUT_PORT = COMMON_PORT,
                          ULTRASONIC_LEFT_SENSOR_OUTPUT_PORT = COMMON_PORT;

static const gpio__port_e ULTRASONIC_MIDDLE_SENSOR_INPUT_PORT = COMMON_PORT,
                          ULTRASONIC_MIDDLE_SENSOR_OUTPUT_PORT = COMMON_PORT;

static const gpio__port_e ULTRASONIC_RIGHT_SENSOR_INPUT_PORT = COMMON_PORT,
                          ULTRASONIC_RIGHT_SENSOR_OUTPUT_PORT = COMMON_PORT;

/**
 * STATIC FUNCTIONS
 */
static void ultrasonic__init_ports_and_pins(void);

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
  ultrasonic__init_ports_and_pins();

  ultrasonic__init(&sensor_left);
  ultrasonic__init(&sensor_middle);
  ultrasonic__init(&sensor_right);
  ultrasonic__init_enable_interrupt_dispatcher();
}

void ultrasonic__update_all_sensors(void) {
  ultrasonic__calculate_distance_from_obstacle(&sensor_left);
  ultrasonic__calculate_distance_from_obstacle(&sensor_middle);
  ultrasonic__calculate_distance_from_obstacle(&sensor_right);
#if ULTRASONIC_DEBUG == 1
  printf("\nLeft = %lf cm .. Mid = %lf cm .. Right = %lf cm\r\n", (double)sensor_left.distance_from_obstacle,
         (double)sensor_middle.distance_from_obstacle, (double)sensor_right.distance_from_obstacle);
#endif
}

void ultrasonic__get_distance_from_all_sensors(dbc_SENSOR_SONARS_s *output) {
  output->SENSOR_SONARS_left = sensor_left.distance_from_obstacle;
  output->SENSOR_SONARS_right = sensor_right.distance_from_obstacle;
  output->SENSOR_SONARS_middle = sensor_middle.distance_from_obstacle;
}

/**
 * STATIC FUNCTION DEFINITIONS
 */
static void ultrasonic__init_ports_and_pins(void) {
  ultrasonic__init_ports_and_pins_left();
  ultrasonic__init_ports_and_pins_middle();
  ultrasonic__init_ports_and_pins_right();
}

static void ultrasonic__init_ports_and_pins_left() {
  sensor_left.echo_input.port_number = ULTRASONIC_LEFT_SENSOR_INPUT_PORT;
  sensor_left.echo_input.pin_number = ULTRASONIC_LEFT_ECHO_INPUT_PIN;
  sensor_left.trig_output.port_number = ULTRASONIC_LEFT_SENSOR_OUTPUT_PORT;
  sensor_left.trig_output.pin_number = ULTRASONIC_LEFT_TRIG_OUTPUT_PIN;
}

static void ultrasonic__init_ports_and_pins_middle() {
  sensor_middle.echo_input.port_number = ULTRASONIC_MIDDLE_SENSOR_INPUT_PORT;
  sensor_middle.echo_input.pin_number = ULTRASONIC_MIDDLE_ECHO_INPUT_PIN;
  sensor_middle.trig_output.port_number = ULTRASONIC_MIDDLE_SENSOR_OUTPUT_PORT;
  sensor_middle.trig_output.pin_number = ULTRASONIC_MIDDLE_TRIG_OUTPUT_PIN;
}

static void ultrasonic__init_ports_and_pins_right() {
  sensor_right.echo_input.port_number = ULTRASONIC_RIGHT_SENSOR_INPUT_PORT;
  sensor_right.echo_input.pin_number = ULTRASONIC_RIGHT_ECHO_INPUT_PIN;
  sensor_right.trig_output.port_number = ULTRASONIC_RIGHT_SENSOR_OUTPUT_PORT;
  sensor_right.trig_output.pin_number = ULTRASONIC_RIGHT_TRIG_OUTPUT_PIN;
}
