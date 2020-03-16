#include "can_bus_initializer.h"

#include "can_bus_constants.h"
#include "can_bus_initializer.h"

/**
 * STATIC FUNCTION DECLARATIONS
 */
static void bus_off_func(uint32_t icr);
static void data_overflow_func(uint32_t icr);

/**
 * DEFINE and STATIC Variables
 */
#define BAUD_RATE_KBPS 100
#define RX_QUEUE_LEN 10
#define TX_QUEUE_LEN 10

void can_bus__initialize() {
  can__init(CAN_PORT, BAUD_RATE_KBPS, RX_QUEUE_LEN, TX_QUEUE_LEN, bus_off_func, data_overflow_func);
  can__bypass_filter_accept_all_msgs();
  can__reset_bus(CAN_PORT);
}

/**
 * STATIC FUNCTION IMPLEMENTATION
 */
static void bus_off_func(uint32_t icr) {
#if DEBUG == 1
  fprintf(stderr, "Bus Off -> 0x%lx\r\n", icr);
#else
  (void)icr;
#endif
}

static void data_overflow_func(uint32_t icr) {
#if DEBUG == 1
  fprintf(stderr, "Data Overflow -> 0x%lx\r\n", icr);
#else
  (void)icr;
#endif
}
