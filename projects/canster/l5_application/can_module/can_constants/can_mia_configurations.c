#include "project.h"

#include <stdint.h>

// Sensor
const uint32_t dbc_mia_threshold_SENSOR_HEARTBEAT = 3000;
const uint32_t dbc_mia_threshold_SENSOR_SONARS = 3000;
const uint32_t dbc_mia_threshold_SENSOR_LIDAR = 3000;
const dbc_SENSOR_HEARTBEAT_s dbc_mia_replacement_SENSOR_HEARTBEAT = {{0}, SENSOR_HEARTBEAT_cmd_REBOOT};
const dbc_SENSOR_SONARS_s dbc_mia_replacement_SENSOR_SONARS = {{0}, 0, 0, 0};
const dbc_SENSOR_LIDAR_s dbc_mia_replacement_SENSOR_LIDAR = {{0}, 0, 0, 0, 0};

// Driver
const uint32_t dbc_mia_threshold_DRIVER_HEARTBEAT = 3000;
const uint32_t dbc_mia_threshold_MOTOR_SPEED = 3000;
const uint32_t dbc_mia_threshold_MOTOR_STEERING = 3000;
const dbc_DRIVER_HEARTBEAT_s dbc_mia_replacement_DRIVER_HEARTBEAT = {{0}, DRIVER_HEARTBEAT_cmd_REBOOT};
const dbc_MOTOR_SPEED_s dbc_mia_replacement_MOTOR_SPEED = {{0}, 0.5};
const dbc_MOTOR_STEERING_s dbc_mia_replacement_MOTOR_STEERING = {{0}, 1};

// Motor
const uint32_t dbc_mia_threshold_MOTOR_HEARTBEAT = 3000;
const uint32_t dbc_mia_threshold_MOTOR_SPEED_FEEDBACK = 3000;

const dbc_MOTOR_HEARTBEAT_s dbc_mia_replacement_MOTOR_HEARTBEAT = {{0}, MOTOR_HEARTBEAT_cmd_REBOOT};
const dbc_MOTOR_SPEED_FEEDBACK_s dbc_mia_replacement_MOTOR_SPEED_FEEDBACK = {{0}, 0.5};

// Geo
const uint32_t dbc_mia_threshold_DRIVER_COORDINATES = 3000;
const uint32_t dbc_mia_threshold_GEO_DEGREE = 3000;
const uint32_t dbc_mia_threshold_GEO_HEARTBEAT = 3000;

const dbc_DRIVER_COORDINATES_s dbc_mia_replacement_DRIVER_COORDINATES;
const dbc_GEO_DEGREE_s dbc_mia_replacement_GEO_DEGREE;
const dbc_GEO_HEARTBEAT_s dbc_mia_replacement_GEO_HEARTBEAT;
