#pragma once
#include <stdbool.h>

#include <stdlib.h>

#include "project.h"

#include "can_bus_constants.h"
#include "who_am_i.h"

#include "can_driver_node.h"

// void can_handler__sensor_heartbeat_manage_mia(void (*handle_func)(dbc_SENSOR_HEARTBEAT_s));
void can_handler__sensor_heartbeat_manage_mia(void);
void can_handler__sensor_transmit(void);

// dbc_SENSOR_HEARTBEAT_s get_dbc_SENSOR_HEARTBEAT_val(void);

void can_handler__receive_sensor_heartbeat(dbc_message_header_t, can__msg_t);

void can_handler__sensor_sonar_receive(dbc_message_header_t, can__msg_t);
