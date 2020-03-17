#pragma once
#include <stdbool.h>

#include <stdlib.h>

#include "project.h"

#include "can_bus_constants.h"
#include "who_am_i.h"

void can_handler__sensor_heartbeat_manage_mia(void (*handle_func)(dbc_SENSOR_HEARTBEAT_s));
void can_handler__sensor_transmit(void);

dbc_SENSOR_HEARTBEAT_s get_dbc_SENSOR_HEARTBEAT_val(void);