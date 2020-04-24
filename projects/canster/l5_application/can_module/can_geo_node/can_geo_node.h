#pragma once

#include <stdbool.h>
#include <stdlib.h>

#include "project.h"
#include "who_am_i.h"

// Transmit
void can_geo__transmit_all_messages(void);

// Mia
void can_geo__geo_heartbeat_mia();
void can_geo__geo_degree_mia();

// Decode
void can_geo__decode_geo_heartbeat(dbc_message_header_t header, uint8_t bytes[8]);
void can_geo__decode_geo_degree(dbc_message_header_t header, uint8_t bytes[8]);

// Getters for all static variables
void can_geo__get_geo_degree(dbc_GEO_DEGREE_s *return_degree);
void can_geo__get_heartbeat(dbc_GEO_HEARTBEAT_s *return_heartbeat);
