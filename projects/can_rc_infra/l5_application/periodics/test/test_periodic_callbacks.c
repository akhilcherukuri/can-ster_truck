#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockboard_io.h"
#include "Mockgpio.h"

#include "Mockcan_bus_initializer.h"
#include "Mockcan_handler.h"

#include "can_mia_configurations.c"

// Include the source we wish to test
#include "periodic_callbacks.c"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {

  can_bus__initialize_Expect();

  gpio_s unused = {};
  board_io__get_led3_ExpectAndReturn(unused);
  gpio__reset_Expect(unused);

  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {
  gpio_s gpio = {};
  board_io__get_led0_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);

#if BOARD_SENSOR_NODE == 1
  can_handler__device_heartbeat_manage_mia_Expect(mia_handle_func);
#endif

#if BOARD_DRIVER_NODE == 1
  can_handler__sensor_sonar_manage_mia_Expect(NULL);
#endif

  periodic_callbacks__1Hz(0);
}

void test__periodic_callbacks__10Hz(void) {
  gpio_s unused = {};
  board_io__get_led1_ExpectAndReturn(unused);
  gpio__toggle_Expect(unused);

  can_handler__transmit_message_10hz_Expect();
  can_handler__handle_all_incoming_messages_Expect();

  periodic_callbacks__10Hz(0);
}
