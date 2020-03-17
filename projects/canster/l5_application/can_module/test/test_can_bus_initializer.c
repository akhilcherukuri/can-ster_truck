#include "unity.h"

#include "Mockcan_bus.h"

#include "can_bus_initializer.c"

void test_can_bus__initialize(void) {
  can__init_ExpectAndReturn(CAN_PORT, BAUD_RATE_KBPS, RX_QUEUE_LEN, TX_QUEUE_LEN, bus_off_func, data_overflow_func,
                            true);
  can__bypass_filter_accept_all_msgs_Expect();
  can__reset_bus_Expect(CAN_PORT);

  can_bus__initialize();
}
