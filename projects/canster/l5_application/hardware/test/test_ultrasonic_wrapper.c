#include "unity.h"

#include "Mockultrasonic.h"

#include "Mockgpio.h"

#include "ultrasonic_wrapper.c"

void setUp() {}
void tearDown() {}

void test_ultrasonic__init_all_sensors(void) {
  ultrasonic__init_Expect(&sensor_left);
  ultrasonic__init_Expect(&sensor_middle);
  ultrasonic__init_Expect(&sensor_right);
  ultrasonic__init_enable_interrupt_dispatcher_Expect();
  ultrasonic__init_all_sensors();
}

void test_ultrasonic__update_all_sensors(void) {

  sensor_middle.distance_from_obstacle = 12.27;
  ultrasonic__calculate_distance_from_obstacle_Expect(NULL);
  ultrasonic__calculate_distance_from_obstacle_IgnoreArg_u_sensor();
  ultrasonic__calculate_distance_from_obstacle_ReturnThruPtr_u_sensor(&sensor_middle);

  sensor_left.distance_from_obstacle = 45.31;
  ultrasonic__calculate_distance_from_obstacle_Expect(NULL);
  ultrasonic__calculate_distance_from_obstacle_IgnoreArg_u_sensor();
  ultrasonic__calculate_distance_from_obstacle_ReturnThruPtr_u_sensor(&sensor_left);

  sensor_right.distance_from_obstacle = 31.54;
  ultrasonic__calculate_distance_from_obstacle_Expect(NULL);
  ultrasonic__calculate_distance_from_obstacle_IgnoreArg_u_sensor();
  ultrasonic__calculate_distance_from_obstacle_ReturnThruPtr_u_sensor(&sensor_right);

  ultrasonic__update_all_sensors();
  TEST_ASSERT_EQUAL_FLOAT(sensor_middle.distance_from_obstacle, 12.27);
}

void test_ultrasonic__init_ports_and_pins_left() {
  ultrasonic__init_ports_and_pins_left();

  TEST_ASSERT_EQUAL_UINT8(sensor_left.echo_input.port_number, ULTRASONIC_LEFT_SENSOR_INPUT_PORT);
  TEST_ASSERT_EQUAL_UINT8(sensor_left.echo_input.pin_number, ULTRASONIC_LEFT_ECHO_INPUT_PIN);
  TEST_ASSERT_EQUAL_UINT8(sensor_left.trig_output.port_number, ULTRASONIC_LEFT_SENSOR_OUTPUT_PORT);
  TEST_ASSERT_EQUAL_UINT8(sensor_left.trig_output.pin_number, ULTRASONIC_LEFT_TRIG_OUTPUT_PIN);
}

void test_ultrasonic__init_ports_and_pins_middle() {
  ultrasonic__init_ports_and_pins_middle();

  TEST_ASSERT_EQUAL_UINT8(sensor_middle.echo_input.port_number, ULTRASONIC_MIDDLE_SENSOR_INPUT_PORT);
  TEST_ASSERT_EQUAL_UINT8(sensor_middle.echo_input.pin_number, ULTRASONIC_MIDDLE_ECHO_INPUT_PIN);
  TEST_ASSERT_EQUAL_UINT8(sensor_middle.trig_output.port_number, ULTRASONIC_MIDDLE_SENSOR_OUTPUT_PORT);
  TEST_ASSERT_EQUAL_UINT8(sensor_middle.trig_output.pin_number, ULTRASONIC_MIDDLE_TRIG_OUTPUT_PIN);
}

void test_ultrasonic__init_ports_and_pins_right() {
  ultrasonic__init_ports_and_pins_middle();

  TEST_ASSERT_EQUAL_UINT8(sensor_right.echo_input.port_number, ULTRASONIC_RIGHT_SENSOR_INPUT_PORT);
  TEST_ASSERT_EQUAL_UINT8(sensor_right.echo_input.pin_number, ULTRASONIC_RIGHT_ECHO_INPUT_PIN);
  TEST_ASSERT_EQUAL_UINT8(sensor_right.trig_output.port_number, ULTRASONIC_RIGHT_SENSOR_OUTPUT_PORT);
  TEST_ASSERT_EQUAL_UINT8(sensor_right.trig_output.pin_number, ULTRASONIC_RIGHT_TRIG_OUTPUT_PIN);
}
