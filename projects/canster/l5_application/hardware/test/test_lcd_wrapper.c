#include "unity.h"

#include "Mocklcd.h"

#include "lcd_wrapper.c"

void setUp() {}
void tearDown() {}

void test_lcd_wrapper__verify_write_response_true(void) {
  bool verified = lcd_wrapper__verify_write_response(LCD_ACK);
  TEST_ASSERT_TRUE(verified);
}

void test_lcd_wrapper__verify_write_response_false(void) {
  bool verified = lcd_wrapper__verify_write_response(LCD_NACK);
  TEST_ASSERT_FALSE(verified);
}

void test_lcd_wrapper__verify_read_response_true(void) {
  bool verified = lcd_wrapper__verify_read_response(LCD_REPORT_OBJECT);
  TEST_ASSERT_TRUE(verified);
}

void test_lcd_wrapper__verify_read_response_false(void) {
  bool verified = lcd_wrapper__verify_read_response(LCD_NACK);
  TEST_ASSERT_FALSE(verified);
}

// READ

void test_lcd_wrapper__read_led_true(void) {

  uint8_t REQUIRED_RESPONSE = LCD_REPORT_OBJECT;
  uint16_t REQUIRED_RESPONSE_DATA = 0x0001;

  lcd__read_Expect(LCD_USERLED, 0, NULL, NULL);
  lcd__read_IgnoreArg_response();
  lcd__read_IgnoreArg_response_data();

  lcd__read_ReturnThruPtr_response(&REQUIRED_RESPONSE);
  lcd__read_ReturnThruPtr_response_data(&REQUIRED_RESPONSE_DATA);

  bool ison = lcd_wrapper__read_led(0);
  TEST_ASSERT_TRUE(ison);
}

void test_lcd_wrapper__read_led_false(void) {

  uint8_t REQUIRED_RESPONSE = LCD_REPORT_OBJECT;
  uint16_t REQUIRED_RESPONSE_DATA = 0x0000;

  lcd__read_Expect(LCD_USERLED, 0, NULL, NULL);
  lcd__read_IgnoreArg_response();
  lcd__read_IgnoreArg_response_data();

  lcd__read_ReturnThruPtr_response(&REQUIRED_RESPONSE);
  lcd__read_ReturnThruPtr_response_data(&REQUIRED_RESPONSE_DATA);

  bool ison = lcd_wrapper__read_led(0);
  TEST_ASSERT_FALSE(ison);
}

void test_lcd_wrapper__read_led_fail(void) {

  uint8_t REQUIRED_RESPONSE = LCD_NACK;
  uint16_t REQUIRED_RESPONSE_DATA = 0x0000;

  lcd__read_Expect(LCD_USERLED, 0, NULL, NULL);
  lcd__read_IgnoreArg_response();
  lcd__read_IgnoreArg_response_data();

  lcd__read_ReturnThruPtr_response(&REQUIRED_RESPONSE);
  lcd__read_ReturnThruPtr_response_data(&REQUIRED_RESPONSE_DATA);

  bool ison = lcd_wrapper__read_led(0);
  TEST_ASSERT_FALSE(ison);
}

void test_lcd_wrapper__read_meter_true(void) {
  uint8_t REQUIRED_RESPONSE = LCD_REPORT_OBJECT;
  uint16_t REQUIRED_RESPONSE_DATA = 0x1234;

  lcd__read_Expect(LCD_METER, 0, NULL, NULL);
  lcd__read_IgnoreArg_response();
  lcd__read_IgnoreArg_response_data();

  lcd__read_ReturnThruPtr_response(&REQUIRED_RESPONSE);
  lcd__read_ReturnThruPtr_response_data(&REQUIRED_RESPONSE_DATA);

  uint16_t value = lcd_wrapper__read_meter(0);
  TEST_ASSERT_EQUAL_UINT16(value, REQUIRED_RESPONSE_DATA);
}

void test_lcd_wrapper__read_meter_false(void) {
  uint8_t REQUIRED_RESPONSE = LCD_NACK;
  uint16_t REQUIRED_RESPONSE_DATA = 0x1234;

  lcd__read_Expect(LCD_METER, 0, NULL, NULL);
  lcd__read_IgnoreArg_response();
  lcd__read_IgnoreArg_response_data();

  lcd__read_ReturnThruPtr_response(&REQUIRED_RESPONSE);
  lcd__read_ReturnThruPtr_response_data(&REQUIRED_RESPONSE_DATA);

  uint16_t value = lcd_wrapper__read_meter(0);
  TEST_ASSERT_EQUAL_UINT16(value, 0);
}

// ! Similar to read_meter code
void test_lcd_wrapper__read_leddigit(void) {}

// WRITE

void test_lcd_wrapper__write_led_true(void) {
  uint8_t REQUIRED_RESPONSE = LCD_ACK;

  lcd__write_Expect(LCD_USERLED, 0, 0x00, true, NULL);
  lcd__write_IgnoreArg_response();
  lcd__write_ReturnThruPtr_response(&REQUIRED_RESPONSE);

  bool value = lcd_wrapper__write_led(0, true);
  TEST_ASSERT_TRUE(value);
}

void test_lcd_wrapper__write_led_false(void) {
  uint8_t REQUIRED_RESPONSE = LCD_NACK;

  lcd__write_Expect(LCD_USERLED, 0, 0x00, true, NULL);
  lcd__write_IgnoreArg_response();
  lcd__write_ReturnThruPtr_response(&REQUIRED_RESPONSE);

  bool value = lcd_wrapper__write_led(0, true);
  TEST_ASSERT_FALSE(value);
}

void test_lcd_wrapper__write_meter_true(void) {
  uint16_t REQUIRED_VALUE = 0x1234;
  uint8_t REQUIRED_RESPONSE = LCD_ACK;

  lcd__write_Expect(LCD_METER, 0, (REQUIRED_VALUE >> 8) & 0xff, REQUIRED_VALUE & 0xff, NULL);
  lcd__write_IgnoreArg_response();
  lcd__write_ReturnThruPtr_response(&REQUIRED_RESPONSE);

  bool value = lcd_wrapper__write_meter(0, REQUIRED_VALUE);
  TEST_ASSERT_TRUE(value);
}

void test_lcd_wrapper__write_meter_false(void) {
  uint16_t REQUIRED_VALUE = 0x1234;
  uint8_t REQUIRED_RESPONSE = LCD_NACK;

  lcd__write_Expect(LCD_METER, 0, (REQUIRED_VALUE >> 8) & 0xff, REQUIRED_VALUE & 0xff, NULL);
  lcd__write_IgnoreArg_response();
  lcd__write_ReturnThruPtr_response(&REQUIRED_RESPONSE);

  bool value = lcd_wrapper__write_meter(0, REQUIRED_VALUE);
  TEST_ASSERT_FALSE(value);
}

// ! Similar to write_meter code
void test_lcd_wrapper__write_leddigit(void) {}
