#include "unity.h"

#include <stdio.h>
#include <string.h>

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockcan_led.h"
#include "Mockclock.h"
#include "Mockgpio.h"
#include "Mocklidar_data_handler.h"
#include "Mockpwm1.h"
#include "Mockqueue.h"
#include "Mockuart.h"

// Include the source we wish to test
#include "lidar.c"

void setUp(void) {}

void tearDown(void) {}

void test__lidar_config_init(void) {
  // gpio_s pin;
  // gpio__construct_with_function_ExpectAnyArgsAndReturn(pin);
  // gpio__construct_with_function_ExpectAnyArgsAndReturn(pin);

  // clock__get_peripheral_clock_hz_Stub();
  // uart__init_ExpectAnyArgs();

  // lidar__config_init();
}

void test__direct_response_correct_order(void) {
  char direct_response_bytes[] = {0xa5, 0x5a, 0x05, 0x00, 0x00, 0x40, 0x81};
  direct_response_counter = 0;

  TEST_ASSERT_EQUAL(0, direct_response_counter);
  for (int i = 0; i < 6; i++) {
    TEST_ASSERT_FALSE(direct_response_check(direct_response_bytes[i]));
    TEST_ASSERT_EQUAL(i + 1, direct_response_counter);
  }
  TEST_ASSERT_TRUE(direct_response_check(direct_response_bytes[6]));
  TEST_ASSERT_EQUAL(6, direct_response_counter);
}

void test__direct_response_out_of_order(void) {
  char direct_response_bytes[] = {0xbe, 0x00, 0x69, 0xf7, 0xa5, 0x5a, 0x05, 0x00, 0x00, 0x40, 0x81};
  direct_response_counter = 0;

  TEST_ASSERT_EQUAL(0, direct_response_counter);
  for (int i = 0; i < 4; i++) {
    TEST_ASSERT_FALSE(direct_response_check(direct_response_bytes[i]));
    TEST_ASSERT_EQUAL(0, direct_response_counter);
  }
  for (int i = 4; i < 10; i++) {
    TEST_ASSERT_FALSE(direct_response_check(direct_response_bytes[i]));
    TEST_ASSERT_EQUAL(i - 3, direct_response_counter);
  }
  TEST_ASSERT_TRUE(direct_response_check(direct_response_bytes[10]));
  TEST_ASSERT_EQUAL(6, direct_response_counter);
}

void test__direct_response_one_incorrect_byte(void) {
  char direct_response_bytes[] = {0xa5, 0x5a, 0x05, 0x06, 0x00, 0x40, 0x81};
  direct_response_counter = 0;

  TEST_ASSERT_EQUAL(0, direct_response_counter);
  for (int i = 0; i < 3; i++) {
    TEST_ASSERT_FALSE(direct_response_check(direct_response_bytes[i]));
    TEST_ASSERT_EQUAL(i + 1, direct_response_counter);
  }
  for (int i = 3; i < 6; i++) {
    TEST_ASSERT_FALSE(direct_response_check(direct_response_bytes[i]));
    TEST_ASSERT_EQUAL(0, direct_response_counter);
  }
}
