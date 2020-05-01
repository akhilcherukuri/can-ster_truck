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

#include "Mockgps.h"
#include "Mockgps_wrapper.h"

// Include the source we wish to test
#include "periodic_callbacks.h"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  can_bus__initialize_Expect();

  gps__init_Expect();
  gps_wrapper__init_Expect();

  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {
  can_handler__handle_all_mia_1hz_Expect();

  gps_wrapper__update_led_when_valid_Expect();

  periodic_callbacks__1Hz(0);
}

void test__periodic_callbacks__10Hz(void) {
  can_handler__handle_all_incoming_messages_10hz_Expect();
  can_handler__transmit_message_10hz_Expect();

  gps__run_once_Expect();

  periodic_callbacks__10Hz(0);
}
