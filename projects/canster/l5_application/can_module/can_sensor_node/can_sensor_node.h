#pragma once

#include <stdbool.h>
#include <stdlib.h>

#include "project.h"
#include "who_am_i.h"

// Transmit
void can_sensor__transmit_all_messages(void);

// Mia
void can_sensor__sensor_heartbeat_mia();
void can_sensor__sensor_sonar_mia();
void can_sensor__sensor_bt_coordinates_mia();
void can_sensor__sensor_lidar_mia();

// Decode
void can_sensor__decode_sensor_heartbeat(dbc_message_header_t header, uint8_t bytes[8]);
void can_sensor__decode_sensor_sonar(dbc_message_header_t header, uint8_t bytes[8]);
void can_sensor__decode_sensor_bt_coordinates(dbc_message_header_t header, uint8_t bytes[8]);
void can_sensor__decode_sensor_lidar(dbc_message_header_t header, uint8_t bytes[8]);

// Getters for all static variables
void can_sensor__get_heartbeat(dbc_SENSOR_HEARTBEAT_s *return_heartbeat);
void can_sensor__get_sensor_sonar(dbc_SENSOR_SONARS_s *return_sonar);
void can_sensor__get_sensor_bt_coordinates(dbc_SENSOR_BT_COORDINATES_s *return_bt_coordinates);
void can_sensor__get_sensor_lidar(dbc_SENSOR_LIDAR_s *return_lidar);