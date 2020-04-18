#include "unity.h"

#include "Mockclock.h"
#include "Mockdelay.h"
#include "Mockgpio.h"
#include "Mocklpc_peripherals.h"
#include "Mocksys_time.h"

#include "lpc40xx.h"
#include "ultrasonic.c"

void setUp() {}
void tearDown() {}

void test_ultrasonic_enable_trigger_pulse() {
  gpio_s sensor_trigger = {};

  gpio__reset_Expect(sensor_trigger);
  delay__us_Expect(2);

  gpio__set_Expect(sensor_trigger);
  delay__us_Expect(10);

  gpio__reset_Expect(sensor_trigger);

  ultrasonic__enable_trigger_pulse(sensor_trigger);
}

void test_ultrasonic_init(void) {
  // Cannot be mocked since hardware pins are called inside (causes unit-test to crash)
}

void test_ultrasonic_calculate_distance_from_obstacle(void) {
  gpio_s sensor_trigger = {};

  gpio__reset_Expect(sensor_trigger);
  delay__us_Expect(2);

  gpio__set_Expect(sensor_trigger);
  delay__us_Expect(10);

  gpio__reset_Expect(sensor_trigger);

  ultrasonic_sensor_s u_sensor = {};
  u_sensor.clock_time_at_falling_edge = 916215.23;
  u_sensor.clock_time_at_rising_edge = 85642.11;
  float computed =
      (u_sensor.clock_time_at_falling_edge - u_sensor.clock_time_at_rising_edge) / distance_cm_conversion_factor;
  ultrasonic__calculate_distance_from_obstacle(&u_sensor);

  TEST_ASSERT_EQUAL_FLOAT(u_sensor.distance_from_obstacle, computed);
}
