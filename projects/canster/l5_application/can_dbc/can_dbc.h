#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "project.h"

/**
 * TODO, Stub functions
 * Add your own can code to these functions
 */
void can_dbc__manage_all_mia(void);               // 1Hz
void can_dbc__handle_all_incoming_messages(void); // 10hz
void can_dbc__transmit_message_10hz(void);        // 10hz

/**
 * EXTERN inside project.h
 */
bool dbc_send_can_message(void *argument_from_dbc_encode_and_send, uint32_t message_id, const uint8_t bytes[8],
                          uint8_t dlc);
