#include "unity.h"

#include "Mocki2c.h"

#include "compass.c"

void setUp() {}
void tearDown() {}

/**
 * Static function for Tests
 */
static void static_compass__read_helper(uint8_t *bytes_to_read, uint32_t number_of_bytes) {
  // Initial Expectations
  i2c__read_slave_data_ExpectAndReturn(I2C_PORT, SLAVE_ADDRESS, 0, NULL, number_of_bytes, true);
  i2c__read_slave_data_IgnoreArg_starting_slave_memory_address();
  i2c__read_slave_data_IgnoreArg_bytes_to_read();

  // Return data for testing
  i2c__read_slave_data_ReturnArrayThruPtr_bytes_to_read(bytes_to_read, number_of_bytes);
}

/**
 * compass_read_helper test
 */
void test_compass_read_helper_one_byte(void) {
  const uint8_t NUMBER_OF_BYTES = 1;

  i2c__read_slave_data_ExpectAndReturn(I2C_PORT, SLAVE_ADDRESS, COMPASS__SOFTWARE_VERSION, NULL, NUMBER_OF_BYTES, true);
  i2c__read_slave_data_IgnoreArg_bytes_to_read();

  uint8_t data[1] = {0xFE};
  i2c__read_slave_data_ReturnThruPtr_bytes_to_read(data);

  uint8_t read_data[1] = {0};
  compass__read_helper(COMPASS__SOFTWARE_VERSION, read_data, NUMBER_OF_BYTES);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(data, read_data, NUMBER_OF_BYTES);
}

void test_compass_read_helper_two_bytes(void) {
  const uint8_t NUMBER_OF_BYTES = 2;

  i2c__read_slave_data_ExpectAndReturn(I2C_PORT, SLAVE_ADDRESS, COMPASS__SOFTWARE_VERSION, NULL, NUMBER_OF_BYTES, true);
  i2c__read_slave_data_IgnoreArg_bytes_to_read();
  i2c__read_slave_data_IgnoreArg_number_of_bytes();

  uint8_t data[2] = {0x80, 0x12};
  i2c__read_slave_data_ReturnArrayThruPtr_bytes_to_read(data, NUMBER_OF_BYTES);

  uint8_t read_data[2] = {0};
  compass__read_helper(COMPASS__SOFTWARE_VERSION, read_data, NUMBER_OF_BYTES);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(data, read_data, NUMBER_OF_BYTES);
}

void test_compass__get_uint8(void) {
  i2c__read_slave_data_ExpectAndReturn(I2C_PORT, SLAVE_ADDRESS, 0, NULL, 1, true);
  i2c__read_slave_data_IgnoreArg_starting_slave_memory_address();
  i2c__read_slave_data_IgnoreArg_bytes_to_read();

  uint8_t data[1] = {0xAA};
  i2c__read_slave_data_ReturnArrayThruPtr_bytes_to_read(data, 1);

  uint8_t uval = compass__get_uint8_t(COMPASS__SOFTWARE_VERSION);
  TEST_ASSERT_EQUAL_UINT8(uval, 0xAA);
}

void test_compass__get_int8(void) {
  i2c__read_slave_data_ExpectAndReturn(I2C_PORT, SLAVE_ADDRESS, 0, NULL, 1, true);
  i2c__read_slave_data_IgnoreArg_starting_slave_memory_address();
  i2c__read_slave_data_IgnoreArg_bytes_to_read();

  uint8_t data[1] = {0xFF};
  i2c__read_slave_data_ReturnArrayThruPtr_bytes_to_read(data, 1);

  int8_t ival = compass__get_int8_t(COMPASS__SOFTWARE_VERSION);
  TEST_ASSERT_EQUAL_INT8(ival, -1);
}

void test_compass__get_uint16(void) {
  i2c__read_slave_data_ExpectAndReturn(I2C_PORT, SLAVE_ADDRESS, 0, NULL, 2, true);
  i2c__read_slave_data_IgnoreArg_starting_slave_memory_address();
  i2c__read_slave_data_IgnoreArg_bytes_to_read();

  uint8_t data[2] = {0x80, 0x44};
  i2c__read_slave_data_ReturnArrayThruPtr_bytes_to_read(data, 2);

  uint16_t uval = compass__get_uint16_t(COMPASS__SOFTWARE_VERSION);
  TEST_ASSERT_EQUAL_UINT16(uval, 0x8044);
}

void test_compass__get_int16(void) {
  i2c__read_slave_data_ExpectAndReturn(I2C_PORT, SLAVE_ADDRESS, 0, NULL, 2, true);
  i2c__read_slave_data_IgnoreArg_starting_slave_memory_address();
  i2c__read_slave_data_IgnoreArg_bytes_to_read();

  uint8_t data[2] = {0xFF, 0xFF};
  i2c__read_slave_data_ReturnArrayThruPtr_bytes_to_read(data, 2);

  uint16_t ival = compass__get_int16_t(COMPASS__SOFTWARE_VERSION);
  TEST_ASSERT_EQUAL_UINT16(ival, -1);
}

/**
 * Used with static function
 */
void test_compass__get_uint16_via_function(void) {
  uint8_t bytes[2] = {0x33, 0x22};
  static_compass__read_helper(bytes, 2);

  uint16_t uval = compass__get_uint16_t(COMPASS__SOFTWARE_VERSION);
  TEST_ASSERT_EQUAL_UINT16(uval, 0x3322);
}

// NOTE, Functions with uint8, int8, uint16 and int16 are similar

/**
 * float testing
 */
void test_compass__read_compass_bearing_16bit_negative_test(void) {
  uint8_t bytes[2] = {0xFF, 0xFE}; // -2 -> -0.2
  static_compass__read_helper(bytes, 2);

  float fval = compass__read_compass_bearing_16bit();
  TEST_ASSERT_EQUAL_FLOAT(fval, -0.2);
}

void test_compass__read_compass_bearing_16bit_positive_test(void) {
  uint8_t bytes[2] = {0x00, 0x08}; // +8 -> 0.8
  static_compass__read_helper(bytes, 2);

  float fval = compass__read_compass_bearing_16bit();
  TEST_ASSERT_EQUAL_FLOAT(fval, 0.8);
}

/**
 * Struct testing
 */
void test_compass__read_gyro(void) {
  uint8_t gbytes1[2] = {0x11, 0x22};
  uint8_t gbytes2[2] = {0x33, 0x44};
  uint8_t gbytes3[2] = {0x55, 0x66};

  static_compass__read_helper(gbytes1, 2);
  static_compass__read_helper(gbytes2, 2);
  static_compass__read_helper(gbytes3, 2);

  compass_gyro_s gyro_data = {};
  compass__read_gyro(&gyro_data);

  TEST_ASSERT_EQUAL_INT16(gyro_data.x, 0x1122);
  TEST_ASSERT_EQUAL_INT16(gyro_data.y, 0x3344);
  TEST_ASSERT_EQUAL_INT16(gyro_data.z, 0x5566);
}