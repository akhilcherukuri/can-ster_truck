#include "periodic_callbacks.h"

#include "board_io.h"
#include "gpio.h"

#include "can_bus_initializer.h"
#include "can_handler.h"

#include "gps.h"
#include "motor_wrapper.h"
#include "ultrasonic_wrapper.h"

#include "bt.h"
#include "bt_wrapper.h"

#include <stdio.h>

/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */
void periodic_callbacks__initialize(void) {
  can_bus__initialize();
  ultrasonic__init_all_sensors();

  bt__init();
}

void periodic_callbacks__1Hz(uint32_t callback_count) {
  can_handler__handle_all_mia();

  bt_wrapper__write_once();
}

void periodic_callbacks__10Hz(uint32_t callback_count) {
  can_handler__handle_all_incoming_messages();
  can_handler__transmit_message_10hz();

  bt_wrapper__read_once();
}

void periodic_callbacks__100Hz(uint32_t callback_count) {}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) { gpio__toggle(board_io__get_led3()); }
