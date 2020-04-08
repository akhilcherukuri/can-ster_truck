#include "unity.h"

#include "Mockclock.h"
#include "Mockdelay.h"
#include "Mockgpio.h"
#include "Mocklpc_peripherals.h"
#include "Mocksys_time.h"

#include "lpc40xx.h"

#include "rpm.c"

void setUp() {}
void tearDown() {}

void test_rpm__init() {
  // Cannot be mocked since hardware pins are called inside (causes unit-test to crash)
}

void speed_calculate_helper(uint16_t test_pulse_count) {
  set_pulse_count(test_pulse_count);
  float expected_speed = (CIRCUMFERENCE_METER * test_pulse_count) * (MPS_TO_KPH_CONVERSION_FACTOR);
  TEST_ASSERT_EQUAL_FLOAT(rpm__calculate_speed_kph(), expected_speed);
}

void test_rpm__calculate_speed_kph(void) {
  uint16_t test_pulse_count = 100;
  speed_calculate_helper(test_pulse_count);
  test_pulse_count = 0;
  speed_calculate_helper(test_pulse_count);
}

void test_rpm__enable_interrupt() {
  // Cannot be mocked since hardware pins are called inside (causes unit-test to crash)
}

void test_rpm_sensor_isr_dispatcher() {
  // Cannot be mocked since hardware pins are called inside (causes unit-test to crash)
}

void test_rpm__clear_interrupt() {
  // Cannot be mocked since hardware pins are called inside (causes unit-test to crash)
}

void test_rpm_sensor_isr_falling(void) {
  uint32_t clock_time_unused;
  sys_time__get_uptime_us_ExpectAndReturn(clock_time_unused);
  rpm_sensor_isr_falling();
}

void test_rpm_sensor_isr_rising(void) {
  uint32_t clock_time_unused;
  sys_time__get_uptime_us_ExpectAndReturn(clock_time_unused);
  rpm_sensor_isr_rising();
}

void test_set_pulse_count() {
  uint16_t pulse = 100;
  set_pulse_count(pulse);
  TEST_ASSERT_EQUAL(pulse, get_pulse_count());
}

void test_get_pulse_count(void) {
  uint16_t pulse = 200, pulse_get;
  set_pulse_count(pulse);
  pulse_get = get_pulse_count();
  TEST_ASSERT_EQUAL(pulse, pulse_get);
}