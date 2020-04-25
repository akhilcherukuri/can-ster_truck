#include "periodic_callbacks.h"

#include "board_io.h"
#include "gpio.h"

#include "can_bus_initializer.h"
#include "can_handler.h"

#include "gps.h"

void periodic_callbacks__initialize(void) {
  can_bus__initialize();

  gps__init();
}

void periodic_callbacks__1Hz(uint32_t callback_count) { can_handler__handle_all_mia(); }

void periodic_callbacks__10Hz(uint32_t callback_count) {
  can_handler__handle_all_incoming_messages();
  can_handler__transmit_message_10hz();

  gps__run_once();
}

void periodic_callbacks__100Hz(uint32_t callback_count) {}

void periodic_callbacks__1000Hz(uint32_t callback_count) {}