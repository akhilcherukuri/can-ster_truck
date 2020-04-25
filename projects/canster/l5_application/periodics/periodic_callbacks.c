#include "periodic_callbacks.h"

#include "board_io.h"
#include "gpio.h"

#include "can_bus_initializer.h"
#include "can_handler.h"

#include "gps.h"

#include <stdio.h>

/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */
void periodic_callbacks__initialize(void) {
  // This method is invoked once when the periodic tasks are created
  can_bus__initialize();

  // gps__init();
}

void periodic_callbacks__1Hz(uint32_t callback_count) {
  can_handler__handle_all_mia_1hz();

  // gps_coordinates_s coordinates = gps__get_coordinates();
  // printf("Coordinates: %f %f\r\n", (double)coordinates.latitude, (double)coordinates.longitude);
  // gps__debug_print_parsed_rmc_data();
}

void periodic_callbacks__10Hz(uint32_t callback_count) {
  can_handler__handle_all_incoming_messages_10hz();
  can_handler__transmit_message_10hz();

  // gps__run_once();
}

void periodic_callbacks__100Hz(uint32_t callback_count) {}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {
  gpio__toggle(board_io__get_led3());
  // Add your code here
}