#include "lcd.h"

#include "gpio.h"
#include "uart.h"

#include "delay.h"

#include "clock.h"

#include "line_buffer.h"

#define LCD_DEBUG 1

#if LCD_DEBUG == 1
#include <stdio.h>
#endif

/**
 * TYPEDEF ENUM and DATATYPESS
 */
// TODO, Add more enums here if needed
typedef enum { READ_OBJ = 0x00, WRITE_OBJ } lcd_command_e;

/**
 * STATIC FUNCTION DECLARATIONS
 */
static void lcd__write_data(uint8_t *data, uint8_t size);

static void lcd__read_response_id(uint8_t *response_id);
static void lcd__read_response_data(uint16_t *response_data);

/**
 * CONSTANT VARIABLES
 */
// UART
static const gpio__port_e UART3_PORT = GPIO__PORT_4;
static const uint8_t UART3_TX = 28;
static const uint8_t UART3_RX = 29;

static const uint32_t LCD_BAUD_RATE = 9600;
static const uart_e LCD_UART = UART__3;

// DATA QUEUE
static const uint32_t RX_QUEUE_SIZE = 50;
static const uint32_t TX_QUEUE_SIZE = 50;

// GPIO Open Drain output
static const gpio__port_e RES_PORT = GPIO__PORT_0;
static const uint8_t RES_PIN = 10;

/**
 * MODULE VARIABLES
 */
// Line Buffer
static line_buffer_s line;
static char line_buffer[100];

/**
 * STATE VARIABLES
 */

/**
 * FUNCTIONS DEFINITIONS
 */
void lcd__init(void) {
  gpio__construct_with_function(UART3_PORT, UART3_TX, GPIO__FUNCTION_2);
  gpio__construct_with_function(UART3_PORT, UART3_RX, GPIO__FUNCTION_2);

  // (1 << 2) is for stop bit is 2
  uart__init_stop_bit(LCD_UART, clock__get_peripheral_clock_hz(), LCD_BAUD_RATE, 0);

  QueueHandle_t rxq_handle = xQueueCreate(RX_QUEUE_SIZE, sizeof(char));
  QueueHandle_t txq_handle = xQueueCreate(TX_QUEUE_SIZE, sizeof(char));
  uart__enable_queues(LCD_UART, rxq_handle, txq_handle);

  line_buffer__init(&line, line_buffer, sizeof(line_buffer));
}

void lcd__reset(void) {
  gpio_s res = gpio__construct_as_output(RES_PORT, RES_PIN);

  gpio__reset(res);
  delay__us(2);

  gpio__set(res);
}

void lcd__write(lcd_object_e object_id, uint8_t object_index, uint8_t msb, uint8_t lsb, uint8_t *response) {
  uint8_t checksum = WRITE_OBJ ^ object_id ^ object_index ^ msb ^ lsb;
  uint8_t buffer[] = {WRITE_OBJ, object_id, object_index, msb, lsb, checksum};

  lcd__write_data(buffer, sizeof(buffer) / sizeof(uint8_t));
  lcd__read_response_id(response);
}

void lcd__read(lcd_object_e object_id, uint8_t object_index, uint8_t *response, uint16_t *response_data) {

  uint8_t checksum = READ_OBJ ^ object_id ^ object_index;
  uint8_t buffer[] = {READ_OBJ, object_id, object_index, checksum};

  lcd__write_data(buffer, sizeof(buffer) / sizeof(uint8_t));
  lcd__read_response_id(response);
  lcd__read_response_data(response_data);
}

/**
 * STATIC FUNCTION DEFINITIONS
 */
static void lcd__read_response_id(uint8_t *response_id) {
  if (response_id == NULL) {
    return;
  }

  // Set an initial value incase timeout occurs
  *response_id = 0;
  const uint8_t maxwait = 30; // ms

  char byte;
  if (uart__get(LCD_UART, &byte, maxwait)) {
    // Add to the response (identifier) data byte
    *response_id = byte;
  }

#if LCD_DEBUG == 1
  printf("I: %x\r\n", byte);
#endif
}

static void lcd__read_response_data(uint16_t *response_data) {
  if (response_data == NULL) {
    return;
  }

  uint8_t data_buffer[5] = {0}; // max size

  char byte;
  // DONE, Empty out the buffer here, depending on the received buffer
  for (int i = 0; i < sizeof(data_buffer) / sizeof(uint8_t); i++) {
    if (uart__get(LCD_UART, &byte, 5)) {
      data_buffer[i] = byte;
#if LCD_DEBUG == 1
      printf("R: %x\r\n", byte);
#endif
    }
  }

  // TODO, Return the entire data bytes out if needed
  *response_data = data_buffer[2] << 8 | data_buffer[3];
}

static void lcd__write_data(uint8_t *data, uint8_t size) {
  for (int i = 0; i < size; i++) {
    uart__put(LCD_UART, data[i], 0);
#if LCD_DEBUG == 1
    printf("%x ", data[i]);
#endif
  }

#if LCD_DEBUG == 1
  printf("\r\n");
#endif
}
