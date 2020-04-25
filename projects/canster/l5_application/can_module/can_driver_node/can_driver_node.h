#include <stdlib.h>

#include "project.h"
#include "who_am_i.h"

// Transmit
void can_driver__transmit_all_messages(void);

// Mia
void can_driver__driver_heartbeat_mia();
void can_driver__driver_steering_mia();

// Decode
void can_driver__decode_driver_heartbeat(dbc_message_header_t header, uint8_t bytes[8]);
void can_driver__decode_driver_steering(dbc_message_header_t header, uint8_t bytes[8]);

// Getters for all static variables
void can_driver__get_driver_steering(dbc_DRIVER_STEERING_s *return_steering);
void can_driver__get_driver_heartbeat(dbc_DRIVER_HEARTBEAT_s *return_heartbeat);