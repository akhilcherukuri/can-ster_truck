#include "gps.h"

#include "gpio.h"
#include "uart.h"

#include "clock.h"

#if GPS_DEBUG == 1
#include <stdio.h>
#endif

/**
 * Definitions and Constants
 */
// UART
static const gpio__port_e UART3_PORT = GPIO__PORT_4;
static const uint8_t UART3_TX = 28;
static const uint8_t UART3_RX = 29;
static const uint32_t GPS_BAUD_RATE = 9600;
static const uart_e GPS_UART = UART__3;

// DATA QUEUE
static const uint32_t RX_QUEUE_SIZE = 200;
static const uint32_t TX_QUEUE_SIZE = 4;

/**
 * Functions
 */
void gps__init() {
  gpio__construct_with_function(UART3_PORT, UART3_TX, GPIO__FUNCTION_2);
  gpio__construct_with_function(UART3_PORT, UART3_RX, GPIO__FUNCTION_2);

  // (1 << 2) is for stop bit is 2
  uart__init_stop_bit(GPS_UART, clock__get_peripheral_clock_hz(), GPS_BAUD_RATE, 0);

  QueueHandle_t rxq_handle = xQueueCreate(RX_QUEUE_SIZE, sizeof(char));
  QueueHandle_t txq_handle = xQueueCreate(TX_QUEUE_SIZE, sizeof(char));
  uart__enable_queues(GPS_UART, rxq_handle, txq_handle);
}

#if GPS_DEBUG == 1

void gps__debug_print_raw_data() {
  char byte;
  while (uart__get(GPS_UART, &byte, 0)) {
    printf("%c -> %x\r\n", byte, byte);
  }
  printf("-----\r\n");
}

#endif
