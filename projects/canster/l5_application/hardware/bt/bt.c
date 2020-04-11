#include "bt.h"

#include "FreeRTOS.h"
#include "queue.h"

#include "clock.h"
#include "gpio.h"

#include "uart.h"

#include "line_buffer.h"

#include <stdio.h>

#define BT_DEBUG 1

static const gpio__port_e UART2_PORT = GPIO__PORT_0;
static const uint8_t UART2_TX = 10;
static const uint8_t UART2_RX = 11;

static const uart_e BT_UART = UART__2;
static const uint32_t BT_BAUD_RATE = 9600;

// DATA QUEUE
static const uint32_t RX_QUEUE_SIZE = 200;
static const uint32_t TX_QUEUE_SIZE = 200;

// Space for the line buffer, and the line buffer data structure instance
static char line_buffer[200];
static line_buffer_s line;

/**
 * Static function declaration
 */
static void bt__absorb_data(void);
static void bt__handle_line(bt__read_func_cb read_func);

// Process lines
static void bt__process_line(char *buffer, char *identifier);

/**
 * Functions
 */
void bt__init(void) {
  gpio__construct_with_function(UART2_PORT, UART2_TX, GPIO__FUNCTION_1);
  gpio__construct_with_function(UART2_PORT, UART2_RX, GPIO__FUNCTION_1);

  uart__init_stop_bit(BT_UART, clock__get_peripheral_clock_hz(), BT_BAUD_RATE, 0);

  QueueHandle_t rxq_handle = xQueueCreate(RX_QUEUE_SIZE, sizeof(char));
  QueueHandle_t txq_handle = xQueueCreate(TX_QUEUE_SIZE, sizeof(char));
  uart__enable_queues(BT_UART, rxq_handle, txq_handle);

  line_buffer__init(&line, line_buffer, sizeof(line_buffer));
}

void bt__read(bt__read_func_cb read_func) {
  bt__absorb_data();
  bt__handle_line(read_func);
}

void bt__write(char *data) {
  for (int i = 0; i < strlen(data); i++) {
    uart__put(BT_UART, data[i], 0);
  }
}

/**
 * Static function
 **/
static void bt__absorb_data(void) {
  char byte;
  while (uart__get(BT_UART, &byte, 0)) {
#if BT_DEBUG == 1
    // Print the data out
    printf("%c : %x\r\n", byte, byte);
#endif
    line_buffer__add_byte(&line, byte);
  }
}

static void bt__handle_line(bt__read_func_cb read_func) {
  char buffer[100] = {0};
  char identifier[10] = {0};
  while (line_buffer__remove_line(&line, buffer, sizeof(buffer))) {

#if BT_DEBUG == 1
    printf("extracted: %s\r\n", buffer);
#endif

    // TODO, Decide shift everything from here to the bt_wrapper module

    // DONE, Process something here
    bt__process_line(buffer, identifier);

#if BT_DEBUG == 1
    printf("Buffer: %s\r\n", buffer);
    printf("Identifier: %s\r\n", identifier);
#endif

    // NOTE, This is to be used by the bt_wrapper function
    if (read_func != NULL) {
      read_func(buffer, identifier);
    }

    // END
  }
}

/**
 * Message receive in the form of $ and ends with \r\n
 * Get identifier here
 * Return the identifier and the rest of the message
 */
static void bt__process_line(char *buffer, char *identifier) {
  if (buffer[0] != '$') {
    return;
  }

  bool found_id = true;
  uint32_t id_counter = 0;

  for (int i = 0; i < strlen(buffer); i++) {
    // ? debugging
    // printf("%x\r\n", buffer[i]);

    // Store the identifier
    if (found_id) {
      if (buffer[i] == ',') {
        found_id = false;
      } else {
        identifier[id_counter] = buffer[i];
        id_counter++;
      }
    }

    // Sanitize for sscanf
    if (buffer[i] == ',') {
      buffer[i] = ' ';
    }

    // END
  }
}
