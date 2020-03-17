#include "project.h"

#include <stdint.h>

// This cannot be 0
const uint32_t dbc_mia_threshold_DRIVER_HEARTBEAT = 3000;
const uint32_t dbc_mia_threshold_SENSOR_HEARTBEAT = 3000;
const uint32_t dbc_mia_threshold_MOTOR_HEARTBEAT = 3000;
const uint32_t dbc_mia_threshold_SENSOR_SONARS = 3000;

// replacement can be 0
const dbc_DRIVER_HEARTBEAT_s dbc_mia_replacement_DRIVER_HEARTBEAT = {{0}, DRIVER_HEARTBEAT_cmd_REBOOT};
const dbc_SENSOR_HEARTBEAT_s dbc_mia_replacement_SENSOR_HEARTBEAT = {{0}, SENSOR_HEARTBEAT_cmd_REBOOT};
const dbc_MOTOR_HEARTBEAT_s dbc_mia_replacement_MOTOR_HEARTBEAT = {{0}, MOTOR_HEARTBEAT_cmd_REBOOT};
const dbc_SENSOR_SONARS_s dbc_mia_replacement_SENSOR_SONARS; // Suggested MIA threshold: (3*50)
