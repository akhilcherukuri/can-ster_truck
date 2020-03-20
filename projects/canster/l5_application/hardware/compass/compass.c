/**
 * NOTE: i2c__initialize is already done in peripherals_init
 */
#include "compass.h"

#include "i2c.h"

/**
 * Static variables
 */
static const i2c_e I2C_PORT = I2C__2;
static const uint8_t SLAVE_ADDRESS = 0xC0;

// Registers
typedef enum {
  COMPASS__SOFTWARE_VERSION, // 0
  COMPASS__BEARING_8BIT,

  COMPASS__BEARING_16BIT_HIGH_BIT, // 2
  COMPASS__BEARING_16BIT_LOW_BIT,

  COMPASS__PITCH_ANGLE_WITH_KF, // 4
  COMPASS__ROLL_ANGLE_WITH_KF,

  COMPASS__MAGNETOMETER_X_AXIS_HIGH_BIT, // 6
  COMPASS__MAGNETOMETER_X_AXIS_LOW_BIT,
  COMPASS__MAGNETOMETER_Y_AXIS_HIGH_BIT,
  COMPASS__MAGNETOMETER_Y_AXIS_LOW_BIT,
  COMPASS__MAGNETOMETER_Z_AXIS_HIGH_BIT,
  COMPASS__MAGNETOMETER_Z_AXIS_LOW_BIT,

  COMPASS__ACCELEROMETER_X_AXIS_HIGH_BIT, // 12
  COMPASS__ACCELEROMETER_X_AXIS_LOW_BIT,
  COMPASS__ACCELEROMETER_Y_AXIS_HIGH_BIT,
  COMPASS__ACCELEROMETER_Y_AXIS_LOW_BIT,
  COMPASS__ACCELEROMETER_Z_AXIS_HIGH_BIT,
  COMPASS__ACCELEROMETER_Z_AXIS_LOW_BIT,

  COMPASS__GYRO_X_AXIS_HIGH_BIT, // 18
  COMPASS__GYRO_X_AXIS_LOW_BIT,
  COMPASS__GYRO_Y_AXIS_HIGH_BIT,
  COMPASS__GYRO_Y_AXIS_LOW_BIT,
  COMPASS__GYRO_Z_AXIS_HIGH_BIT,
  COMPASS__GYRO_Z_AXIS_LOW_BIT,

  COMPASS__TEMPERATURE_HIGH_BIT, // 24
  COMPASS__TEMPERATURE_LOW_BIT,

  COMPASS__PITCH_ANGLE_NO_KF, // 26
  COMPASS__ROLL_ANGLE_NO_KF,
} compass_register_e;

/**
 * Static function declarations
 */
static bool compass__read_helper(compass_register_e starting_slave_address, uint8_t *bytes_to_read,
                                 uint32_t number_of_bytes);
static uint8_t compass__get_uint8_t(compass_register_e starting_slave_address);
static uint16_t compass__get_uint16_t(compass_register_e starting_slave_address);

static int8_t compass__get_int8_t(compass_register_e starting_slave_address);
static int16_t compass__get_int16_t(compass_register_e starting_slave_address);

/**
 * Header function implementation
 */
uint8_t compass__read_software_version(void) { return compass__get_uint8_t(COMPASS__SOFTWARE_VERSION); }
uint8_t compass__read_compass_bearing_8bit(void) { return compass__get_uint8_t(COMPASS__BEARING_8BIT); }

int8_t compass__read_pitch_angle_with_kf(void) { return compass__get_int8_t(COMPASS__PITCH_ANGLE_WITH_KF); }
int8_t compass__read_roll_angle_with_kf(void) { return compass__get_int8_t(COMPASS__ROLL_ANGLE_WITH_KF); }
int8_t compass__read_pitch_angle_no_kf(void) { return compass__get_int8_t(COMPASS__PITCH_ANGLE_NO_KF); }
int8_t compass__read_roll_angle_no_kf(void) { return compass__get_int8_t(COMPASS__ROLL_ANGLE_NO_KF); }

// 0 - 3599, 0 - 359.9 degrees
// NOTE: Warning: Implicit conversion from float to double (cast)
float compass__read_compass_bearing_16bit(void) {
  return (((double)compass__get_int16_t(COMPASS__BEARING_16BIT_HIGH_BIT)) / 10.0);
}

int16_t compass__read_temperature(void) { return compass__get_int16_t(COMPASS__TEMPERATURE_HIGH_BIT); }

void compass__read_magnetometer(compass_magnetometer_s *magnetometer_data) {
  magnetometer_data->x = compass__get_int16_t(COMPASS__MAGNETOMETER_X_AXIS_HIGH_BIT);
  magnetometer_data->y = compass__get_int16_t(COMPASS__MAGNETOMETER_Y_AXIS_HIGH_BIT);
  magnetometer_data->z = compass__get_int16_t(COMPASS__MAGNETOMETER_Z_AXIS_HIGH_BIT);
}

void compass__read_accelerometer(compass_accelerometer_s *accelerometer_data) {
  accelerometer_data->x = compass__get_int16_t(COMPASS__ACCELEROMETER_X_AXIS_HIGH_BIT);
  accelerometer_data->y = compass__get_int16_t(COMPASS__ACCELEROMETER_Y_AXIS_HIGH_BIT);
  accelerometer_data->z = compass__get_int16_t(COMPASS__ACCELEROMETER_Z_AXIS_HIGH_BIT);
}
void compass__read_gyro(compass_gyro_s *gyro_data) {
  gyro_data->x = compass__get_int16_t(COMPASS__GYRO_X_AXIS_HIGH_BIT);
  gyro_data->y = compass__get_int16_t(COMPASS__GYRO_Y_AXIS_HIGH_BIT);
  gyro_data->z = compass__get_int16_t(COMPASS__GYRO_Z_AXIS_HIGH_BIT);
}

/**
 * Static function implementation
 */
static bool compass__read_helper(compass_register_e starting_slave_address, uint8_t *bytes_to_read,
                                 uint32_t number_of_bytes) {
  return i2c__read_slave_data(I2C_PORT, SLAVE_ADDRESS, (uint8_t)starting_slave_address, bytes_to_read, number_of_bytes);
}

static uint8_t compass__get_uint8_t(compass_register_e starting_slave_address) {
  uint8_t data[1] = {0};
  uint8_t rvalue = 0;
  if (compass__read_helper(starting_slave_address, data, 1)) {
    rvalue = data[0];
  }
  return rvalue;
}

// data[0] is MSB, data[1] is LSB
static uint16_t compass__get_uint16_t(compass_register_e starting_slave_address) {
  uint8_t data[2] = {0};
  uint16_t rvalue = 0;
  if (compass__read_helper(starting_slave_address, data, 2)) {
    rvalue = (data[0] << 8) | data[1];
  }
  return rvalue;
}

static int8_t compass__get_int8_t(compass_register_e starting_slave_address) {
  return ((int8_t)compass__get_uint8_t(starting_slave_address));
}

static int16_t compass__get_int16_t(compass_register_e starting_slave_address) {
  return ((int16_t)compass__get_uint16_t(starting_slave_address));
}
