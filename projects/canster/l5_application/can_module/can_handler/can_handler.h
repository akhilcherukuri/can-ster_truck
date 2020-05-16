#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "project.h"
#include "who_am_i.h"

void can_handler__handle_all_mia(void);

void can_handler__handle_all_incoming_messages(void);
void can_handler__transmit_message_10hz(uint32_t);

/**
 * EXTERN inside project.h
 */
bool dbc_send_can_message(void *argument_from_dbc_encode_and_send, uint32_t message_id, const uint8_t bytes[8],
                          uint8_t dlc);
