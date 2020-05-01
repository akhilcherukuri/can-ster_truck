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
void can_geo__geo_destination_reached_mia();

// Decode
void can_geo__decode_geo_heartbeat(dbc_message_header_t header, uint8_t bytes[8]);
void can_geo__decode_geo_degree(dbc_message_header_t header, uint8_t bytes[8]);
void can_geo__decode_geo_destination_reached(dbc_message_header_t header, uint8_t bytes[8]);

// Debug
void can_geo__decode_geo_current_coordinates(dbc_message_header_t header, uint8_t bytes[8]);
void can_geo__decode_geo_debug(dbc_message_header_t header, uint8_t bytes[8]);

// Getters for all static variables
void can_geo__get_geo_degree(dbc_GEO_DEGREE_s *return_degree);
void can_geo__get_heartbeat(dbc_GEO_HEARTBEAT_s *return_heartbeat);
void can_geo__get_geo_destination_reached(dbc_GEO_DESTINATION_REACHED_s *return_destination_reached);

// Debug
void can_geo__get_geo_current_coordinates(dbc_GEO_CURRENT_COORDINATES_s *return_current_coordinates);
void can_geo__get_geo_debug(dbc_GEO_DEBUG_s *return_debug);
