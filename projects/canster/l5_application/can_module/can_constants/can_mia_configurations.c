#include "project.h"

#include <stdint.h>

// Sensor
const uint32_t dbc_mia_threshold_SENSOR_HEARTBEAT = 3000;
const uint32_t dbc_mia_threshold_SENSOR_SONARS = 3000;
const uint32_t dbc_mia_threshold_SENSOR_BT_COORDINATES = 3000;
const uint32_t dbc_mia_threshold_SENSOR_LIDAR = 3000;

const dbc_SENSOR_HEARTBEAT_s dbc_mia_replacement_SENSOR_HEARTBEAT;
const dbc_SENSOR_SONARS_s dbc_mia_replacement_SENSOR_SONARS;
const dbc_SENSOR_BT_COORDINATES_s dbc_mia_replacement_SENSOR_BT_COORDINATES;
const dbc_SENSOR_LIDAR_s dbc_mia_replacement_SENSOR_LIDAR;

// Driver
const uint32_t dbc_mia_threshold_DRIVER_HEARTBEAT = 3000;
const uint32_t dbc_mia_threshold_DRIVER_STEERING = 3000;

const dbc_DRIVER_HEARTBEAT_s dbc_mia_replacement_DRIVER_HEARTBEAT;
const dbc_DRIVER_STEERING_s dbc_mia_replacement_DRIVER_STEERING;

// Motor
const uint32_t dbc_mia_threshold_MOTOR_HEARTBEAT = 3000;
const uint32_t dbc_mia_threshold_MOTOR_INFO = 3000;
const uint32_t dbc_mia_threshold_MOTOR_DEBUG = 3000;

const dbc_MOTOR_HEARTBEAT_s dbc_mia_replacement_MOTOR_HEARTBEAT;
const dbc_MOTOR_INFO_s dbc_mia_replacement_MOTOR_INFO;
const dbc_MOTOR_DEBUG_s dbc_mia_replacement_MOTOR_DEBUG;

// Geo
const uint32_t dbc_mia_threshold_GEO_DEGREE = 3000;
const uint32_t dbc_mia_threshold_GEO_HEARTBEAT = 3000;
const uint32_t dbc_mia_threshold_GEO_DESTINATION_REACHED = 3000;
const uint32_t dbc_mia_threshold_GEO_CURRENT_COORDINATES = 3000;
const uint32_t dbc_mia_threshold_GEO_DEBUG = 3000;

const dbc_GEO_DEGREE_s dbc_mia_replacement_GEO_DEGREE;
const dbc_GEO_HEARTBEAT_s dbc_mia_replacement_GEO_HEARTBEAT;
const dbc_GEO_DESTINATION_REACHED_s dbc_mia_replacement_GEO_DESTINATION_REACHED;
const dbc_GEO_CURRENT_COORDINATES_s dbc_mia_replacement_GEO_CURRENT_COORDINATES;
const dbc_GEO_DEBUG_s dbc_mia_replacement_GEO_DEBUG;
