#include "unity.h"

#include "Mockgpio.h"
#include "Mockgps.h"

#include "gps_wrapper.c"

void setUp() {}
void tearDown() {}

void test_gps_wrapper__init() {
  gpio__construct_as_output_ExpectAndReturn(GPIO__PORT_0, 6, valid_gpio);
  gpio__reset_Expect(valid_gpio);

  gps_wrapper__init();
}

void test_gps_wrapper__update_led_when_valid_true() {
  gps__is_valid_ExpectAndReturn(true);
  gpio__set_Expect(valid_gpio);

  gps_wrapper__update_led_when_valid();
}

void test_gps_wrapper__update_led_when_valid_false() {
  gps__is_valid_ExpectAndReturn(false);
  gpio__reset_Expect(valid_gpio);

  gps_wrapper__update_led_when_valid();
}
