#pragma once

#include <stdbool.h>
#include <stdlib.h>

#include "project.h"
#include "who_am_i.h"

// Mia
void can_motor__motor_heartbeat_mia();
void can_motor__motor_speed_feedback_mia();

// Transmit
void can_motor__transmit_all_messages(void);

// Decode
void can_motor__decode_motor_heartbeat(dbc_message_header_t header, uint8_t bytes[8]);
void can_motor__decode_motor_speed_feedback(dbc_message_header_t header, uint8_t bytes[8]);

// Getters for all static variables
dbc_MOTOR_HEARTBEAT_s can_motor__get_heartbeat();
dbc_MOTOR_SPEED_FEEDBACK_s can_motor__get_motor_speed_feedback();