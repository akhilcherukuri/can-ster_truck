#pragma once

#include <stdio.h>

#include "project.h"

#include "can_bus_constants.h"
#include "who_am_i.h"

void can_handler__motor_heartbeat_manage_mia(void (*handle_func)(dbc_MOTOR_HEARTBEAT_s));
void can_handler__motor_transmit(void);

// dbc_MOTOR_HEARTBEAT_s get_dbc_MOTOR_HEARTBEAT_val(void);
