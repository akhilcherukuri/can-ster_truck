#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "project.h"

typedef void (*can_void_func_t)(uint32_t);

void can_handler__device_heartbeat_manage_mia(void (*handle_func)(dbc_DRIVER_HEARTBEAT_s));
void can_handler__sensor_heartbeat_manage_mia(void (*handle_func)(dbc_SENSOR_HEARTBEAT_s));
void can_handler__motor_heartbeat_manage_mia(void (*handle_func)(dbc_MOTOR_HEARTBEAT_s));

void can_handler__handle_all_incoming_messages(void);
void can_handler__transmit_message_10hz();

/**
 * EXTERN inside project.h
 */
bool dbc_send_can_message(void *argument_from_dbc_encode_and_send, uint32_t message_id, const uint8_t bytes[8],
                          uint8_t dlc);

#ifndef BOARDS_DEFINED

#define BOARDS_DEFINED
#define BOARD_SENSOR_NODE 0 // USB 0
#define BOARD_DRIVER_NODE 1 // USB 1
#define BOARD_MOTOR_NODE 0  // USB 2

#if BOARD_SENSOR_NODE + BOARD_DRIVER_NODE + BOARD_MOTOR_NODE != 1
#error "Cannot compile since only one Node must be made active during compilation"
#endif

#endif /* BOARDS_DEFINED */