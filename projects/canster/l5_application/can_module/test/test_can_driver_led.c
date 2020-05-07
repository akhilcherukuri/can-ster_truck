#include "unity.h"

#include "Mockgpio.h"

#include "can_driver_led.c"

void setUp() {}
void tearDown() {}

void test_can_driver_led__init() {
  gpio__construct_as_output_ExpectAndReturn(GPIO_DRIVER_PORT, GPIO_MOTOR_PIN, motor_gpio);
  gpio__construct_as_output_ExpectAndReturn(GPIO_DRIVER_PORT, GPIO_GEO_PIN, geo_gpio);
  gpio__construct_as_output_ExpectAndReturn(GPIO_DRIVER_PORT, GPIO_SENSOR_PIN, sensor_gpio);

  can_driver_led__init();
}

// Set
void test_can_driver_led__set_motor_heartbeat_mia() {
  gpio__set_Expect(motor_gpio);
  can_driver_led__set_motor_heartbeat_mia();
}

void test_can_driver_led__set_geo_heartbeat_mia() {
  gpio__set_Expect(geo_gpio);
  can_driver_led__set_geo_heartbeat_mia();
}

void test_can_driver_led__set_sensor_heartbeat_mia() {
  gpio__set_Expect(sensor_gpio);
  can_driver_led__set_sensor_heartbeat_mia();
}

// Reset
void test_can_driver_led__reset_motor_heartbeat_mia() {}
void test_can_driver_led__reset_geo_heartbeat_mia() {}
void test_can_driver_led__reset_sensor_heartbeat_mia() {}
