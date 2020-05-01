#pragma once

#include <stdbool.h>
#include <stdlib.h>

#include "project.h"
#include "who_am_i.h"

// Mia
void can_motor__motor_heartbeat_mia();
void can_motor__motor_info_mia();

// Transmit
void can_motor__transmit_all_messages(void);

// Decode
void can_motor__decode_motor_heartbeat(dbc_message_header_t header, uint8_t bytes[8]);
void can_motor__decode_motor_info(dbc_message_header_t header, uint8_t bytes[8]);

// DEBUG
void can_motor__decode_motor_debug(dbc_message_header_t header, uint8_t bytes[8]);

// Getters for all static variables
void can_motor__get_heartbeat(dbc_MOTOR_HEARTBEAT_s *return_heartbeat);
void can_motor__get_motor_info(dbc_MOTOR_INFO_s *return_info);

// DEBUG
void can_motor__get_motor_debug(dbc_MOTOR_DEBUG_s *return_debug);