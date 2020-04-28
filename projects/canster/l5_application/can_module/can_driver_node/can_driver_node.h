#include <stdlib.h>

#include "project.h"
#include "who_am_i.h"

// Transmit
void can_driver__transmit_all_messages(void);

// Mia
void can_driver__driver_heartbeat_mia();
void can_driver__motor_speed_mia();
void can_driver__motor_steering_mia();
void can_driver__driver_coordinates_mia();

// Decode
void can_driver__decode_driver_heartbeat(dbc_message_header_t header, uint8_t bytes[8]);
void can_driver__decode_motor_speed(dbc_message_header_t header, uint8_t bytes[8]);
void can_driver__decode_motor_steering(dbc_message_header_t header, uint8_t bytes[8]);
void can_driver__decode_driver_coordinates(dbc_message_header_t header, uint8_t bytes[8]);

// Getters for all static variables
// ! NO SETTERS
// ! DO NOT DISCARD THE CONST QUALIFIER
dbc_MOTOR_STEERING_s can_driver__get_driver_steering();
dbc_MOTOR_SPEED_s can_driver__get_driver_required_motor_speed();
dbc_DRIVER_HEARTBEAT_s can_driver__get_driver_heartbeat();
dbc_DRIVER_COORDINATES_s can_driver__get_driver_coordinates();