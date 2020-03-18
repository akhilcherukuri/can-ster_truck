#pragma once

#include <stdbool.h>
#include <stdlib.h>

#include "project.h"
#include "who_am_i.h"

void can_sensor__handle_all_mia(void);
void can_sensor__receive_all_messages(dbc_message_header_t header, uint8_t bytes[8]);
void can_sensor__transmit_all_messages(void);
