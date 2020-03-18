#pragma once

#include <stdbool.h>
#include <stdlib.h>

#include "project.h"
#include "who_am_i.h"

void can_driver__transmit_all_messages(void);

// Mia
void can_driver__driver_heartbeat_mia();
void can_driver__sensor_sonar_mia();
void can_driver__motor_speed_feedback_mia();

void can_driver__decode_driver_heartbeat(dbc_message_header_t header, uint8_t bytes[8]);
void can_driver__decode_sensor_sonar(dbc_message_header_t header, uint8_t bytes[8]);
void can_driver__decode_motor_speed_feedback(dbc_message_header_t header, uint8_t bytes[8]);

const dbc_SENSOR_SONARS_s *can_sensor__get_sensor_sonar();
const dbc_DRIVER_HEARTBEAT_s *can_driver__get_heartbeat();
const dbc_MOTOR_SPEED_FEEDBACK_s *can_motor__get_motor_speed_feedback();
