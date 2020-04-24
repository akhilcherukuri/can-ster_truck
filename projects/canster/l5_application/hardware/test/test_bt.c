#include "unity.h"

#include <stdio.h>

#include "MockFreeRTOS.h"
#include "Mockqueue.h"

#include "Mockclock.h"
#include "Mockgpio.h"

#include "Mockuart.h"

#include "line_buffer.h"

#include "bt.c"

void setUp() {
  memset(line_buffer, 0, sizeof(line_buffer));
  line_buffer__init(&line, line_buffer, sizeof(line_buffer));
}
void tearDown() {}

void test_bt__init(void) {
  gpio_s unused;
  gpio__construct_with_function_ExpectAndReturn(UART2_PORT, UART2_TX, GPIO__FUNCTION_1, unused);
  gpio__construct_with_function_ExpectAndReturn(UART2_PORT, UART2_RX, GPIO__FUNCTION_1, unused);

  uint32_t value;
  clock__get_peripheral_clock_hz_ExpectAndReturn(value);
  uart__init_stop_bit_Expect(BT_UART, value, BT_BAUD_RATE, 0);

  QueueHandle_t rxq_handle;
  QueueHandle_t txq_handle;
  xQueueCreate_ExpectAndReturn(RX_QUEUE_SIZE, sizeof(char), rxq_handle);
  xQueueCreate_ExpectAndReturn(TX_QUEUE_SIZE, sizeof(char), txq_handle);

  bool bunused;
  uart__enable_queues_ExpectAndReturn(BT_UART, rxq_handle, txq_handle, bunused);

  bt__init();
}

void test_bt__write(void) {
  char input[] = "$loc,123,234";

  bool bunused;
  for (int i = 0; i < strlen(input); i++) {
    uart__put_ExpectAndReturn(BT_UART, input[i], 0, bunused);
  }

  bt__write(input);
}

void test_bt__absorb_data_single(void) {
  char byte = 'a';

  uart__get_ExpectAndReturn(BT_UART, &byte, 0, true);
  uart__get_IgnoreArg_input_byte();
  uart__get_ReturnThruPtr_input_byte(&byte);

  uart__get_ExpectAndReturn(BT_UART, &byte, 0, false);
  uart__get_IgnoreArg_input_byte();

  bt__absorb_data();

  TEST_ASSERT_EQUAL_UINT32(line.write_index, 1);
}

void test_bt__absorb_data_multiple(void) {
  char data[] = "$loc,123,234\r\n";

  char byte;
  for (int i = 0; i < strlen(data); i++) {
    uart__get_ExpectAndReturn(BT_UART, NULL, 0, true);
    uart__get_IgnoreArg_input_byte();
    uart__get_ReturnThruPtr_input_byte(&data[i]);
  }

  uart__get_ExpectAndReturn(BT_UART, NULL, 0, false);
  uart__get_IgnoreArg_input_byte();

  bt__absorb_data();

  // TEST_ASSERT_EQUAL_UINT32(line.write_index, 1);
  char buffer[30] = {0};

  bool isremoved = line_buffer__remove_line(&line, buffer, sizeof(buffer));
  TEST_ASSERT_TRUE(isremoved);
  TEST_ASSERT_EQUAL_STRING(buffer, "$loc,123,234\r");
}

void test_bt__process_line(void) {
  char identifier[10] = {0};

  char data[] = "$loc,123,234\r";
  bt__process_line(data, identifier);

  TEST_ASSERT_EQUAL_STRING("$loc 123 234\r", data);
  TEST_ASSERT_EQUAL_STRING("$loc", identifier);
}
