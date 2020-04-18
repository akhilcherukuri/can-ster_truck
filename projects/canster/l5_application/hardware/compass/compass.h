
/**
 * NOTE: KF -> Kalman Filter
 * Datasheet -> https://www.robot-electronics.co.uk/htm/cmps11i2c.htm
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  int16_t x, y, z;
} compass_magnetometer_s;

typedef struct {
  int16_t x, y, z;
} compass_accelerometer_s;

typedef struct {
  int16_t x, y, z;
} compass_gyro_s;

uint8_t compass__read_software_version(void);
uint8_t compass__read_compass_bearing_8bit(void);

int8_t compass__read_pitch_angle_with_kf(void);
int8_t compass__read_roll_angle_with_kf(void);
int8_t compass__read_pitch_angle_no_kf(void);
int8_t compass__read_roll_angle_no_kf(void);

// 0 - 3599, 0 - 359.9 degrees
float compass__read_compass_bearing_16bit(void);

int16_t compass__read_temperature(void);

void compass__read_magnetometer(compass_magnetometer_s *magnetometer_data);
void compass__read_accelerometer(compass_accelerometer_s *accelerometer_data);
void compass__read_gyro(compass_gyro_s *gyro_data);
