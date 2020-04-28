#include "unity.h"

#include "Mockgpio.h"
#include "Mockuart.h"

#include "Mockclock.h"
#include "Mockdelay.h"

#include "Mockqueue.h"

#include "line_buffer.h"

#include "lcd.c"

void setUp() {}
void tearDown() {}

void test_lcd_write_data(void) {
  uint8_t data[] = {0x11, 0x22, 0x33, 0x44};

  for (int i = 0; i < sizeof(data) / sizeof(uint8_t); i++) {
    uart__put_ExpectAndReturn(LCD_UART, data[i], 0, true);
  }

  lcd__write_data(data, sizeof(data) / sizeof(uint8_t));
}

// Does nothing
void test_lcd_read_response_id_when_null_param(void) { lcd__read_response_id(NULL); }

void test_lcd_read_response_id_get_data(void) {
  char byte = 0xFE;
  uart__get_ExpectAndReturn(LCD_UART, NULL, 30, true);
  uart__get_IgnoreArg_input_byte();
  uart__get_ReturnThruPtr_input_byte(&byte);

  uint8_t response_id;
  lcd__read_response_id(&response_id);
  TEST_ASSERT_EQUAL_UINT8(response_id, byte);
}

void test_lcd_read_response_id_timeout(void) {
  uart__get_ExpectAndReturn(LCD_UART, NULL, 30, false);
  uart__get_IgnoreArg_input_byte();

  uint8_t response_id;
  lcd__read_response_id(&response_id);
  TEST_ASSERT_EQUAL_UINT8(response_id, 0x00);
}

void test_lcd_read_response_data_when_null_param(void) { lcd__read_response_data(NULL); }

void test_lcd_read_response_data_get_data(void) {
  uint8_t checksum = 0x05 ^ 0x00 ^ 0x01 ^ 0x02;
  const uint8_t input_byte[5] = {0x05, 0x00, 0x01, 0x02, checksum};
  for (int i = 0; i < sizeof(input_byte) / sizeof(uint8_t); i++) {
    uart__get_ExpectAndReturn(LCD_UART, NULL, 5, true);
    uart__get_IgnoreArg_input_byte();
    uart__get_ReturnThruPtr_input_byte((char *)&input_byte[i]);
  }

  uint16_t response_data;
  lcd__read_response_data(&response_data);
  TEST_ASSERT_EQUAL_UINT16(response_data, (0x01 << 8) | (0x02));
}

// This condition returns 0x00 on failure
// NOTE, can also return 0x00 on true
// Always verify with response_id received before using the response_data value
void test_lcd_read_response_data_timeout(void) {
  for (int i = 0; i < 5; i++) {
    uart__get_ExpectAndReturn(LCD_UART, NULL, 5, false);
    uart__get_IgnoreArg_input_byte();
  }

  uint16_t response_data;
  lcd__read_response_data(&response_data);
  TEST_ASSERT_EQUAL_UINT16(response_data, 0x00);
}

/**
 * FUNCTIONS
 */
void test_lcd_init(void) {
  gpio_s gunused;
  gpio__construct_with_function_ExpectAndReturn(UART3_PORT, UART3_TX, GPIO__FUNCTION_2, gunused);
  gpio__construct_with_function_ExpectAndReturn(UART3_PORT, UART3_RX, GPIO__FUNCTION_2, gunused);

  uint32_t clock;
  clock__get_peripheral_clock_hz_ExpectAndReturn(clock);
  uart__init_stop_bit_Expect(LCD_UART, clock, LCD_BAUD_RATE, 0);

  QueueHandle_t rxq, txq;
  xQueueCreate_ExpectAndReturn(RX_QUEUE_SIZE, sizeof(char), rxq);
  xQueueCreate_ExpectAndReturn(TX_QUEUE_SIZE, sizeof(char), txq);

  bool unused;
  uart__enable_queues_ExpectAndReturn(LCD_UART, rxq, txq, unused);

  line_buffer__init(&line, line_buffer, sizeof(line_buffer));

  lcd__init();
}

void test_lcd_reset(void) {
  gpio_s res;
  gpio__construct_as_output_ExpectAndReturn(RES_PORT, RES_PIN, res);

  gpio__reset_Expect(res);
  delay__us_Expect(2);

  gpio__set_Expect(res);

  lcd__reset();
}

void test_lcd_write_true(void) {
  uint8_t checksum = WRITE_OBJ ^ LCD_USERLED ^ 0 ^ 0x00 ^ 0x01;
  uint8_t buffer[] = {WRITE_OBJ, LCD_USERLED, 0, 0x00, 0x01, checksum};
  for (int i = 0; i < sizeof(buffer) / sizeof(uint8_t); i++) {
    uart__put_ExpectAndReturn(LCD_UART, buffer[i], 0, true);
  }

  char input_byte = LCD_ACK;
  uart__get_ExpectAndReturn(LCD_UART, NULL, 30, true);
  uart__get_IgnoreArg_input_byte();
  uart__get_ReturnThruPtr_input_byte(&input_byte);

  uint8_t response;
  lcd__write(LCD_USERLED, 0, 0x00, 0x01, &response);
  TEST_ASSERT_EQUAL_UINT8(response, LCD_ACK);
}

void test_lcd_write_false(void) {
  uint8_t checksum = WRITE_OBJ ^ LCD_USERLED ^ 0 ^ 0x00 ^ 0x01;
  uint8_t buffer[] = {WRITE_OBJ, LCD_USERLED, 0, 0x00, 0x01, checksum};
  for (int i = 0; i < sizeof(buffer) / sizeof(uint8_t); i++) {
    uart__put_ExpectAndReturn(LCD_UART, buffer[i], 0, true);
  }

  char input_byte = LCD_NACK;
  uart__get_ExpectAndReturn(LCD_UART, NULL, 30, true);
  uart__get_IgnoreArg_input_byte();
  uart__get_ReturnThruPtr_input_byte(&input_byte);

  uint8_t response;
  lcd__write(LCD_USERLED, 0, 0x00, 0x01, &response);
  TEST_ASSERT_EQUAL_UINT8(response, LCD_NACK);
}

void test_lcd_write_timeout(void) {
  uint8_t checksum = WRITE_OBJ ^ LCD_USERLED ^ 0 ^ 0x00 ^ 0x01;
  uint8_t buffer[] = {WRITE_OBJ, LCD_USERLED, 0, 0x00, 0x01, checksum};
  for (int i = 0; i < sizeof(buffer) / sizeof(uint8_t); i++) {
    uart__put_ExpectAndReturn(LCD_UART, buffer[i], 0, true);
  }

  uart__get_ExpectAndReturn(LCD_UART, NULL, 30, false);
  uart__get_IgnoreArg_input_byte();

  uint8_t response;
  lcd__write(LCD_USERLED, 0, 0x00, 0x01, &response);
  TEST_ASSERT_EQUAL_UINT8(response, 0x00);
}

// Similar to lcd_write function test
// * NOTE, The individual static functions have been tested above
void test_lcd_read(void) {}
