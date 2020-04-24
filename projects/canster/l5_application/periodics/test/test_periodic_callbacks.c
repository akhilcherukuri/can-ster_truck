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

// Include the source we wish to test
#include "periodic_callbacks.h"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  can_bus__initialize_Expect();

  gps__init_Expect();

  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {
  // gps__run_once_Expect();
  can_handler__handle_all_mia_Expect();

  periodic_callbacks__1Hz(0);
}

void test__periodic_callbacks__10Hz(void) {
  can_handler__handle_all_incoming_messages_Expect();
  can_handler__transmit_message_10hz_Expect();

  gps__run_once_Expect();

  periodic_callbacks__10Hz(0);
}