#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <stdlib.h>
// Check if includes are necessary

#include "project.h"

#include "can_bus_constants.h"
#include "who_am_i.h"

void can_handler__sensor_heartbeat_manage_mia(void (*handle_func)(dbc_SENSOR_HEARTBEAT_s));
void can_handler__sensor_transmit(void);

dbc_SENSOR_HEARTBEAT_s get_dbc_SENSOR_HEARTBEAT_val_from_driver_node_c_file(void);
