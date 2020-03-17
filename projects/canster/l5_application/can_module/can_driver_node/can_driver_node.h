#include "project.h"
#include <stdio.h>

#include "can_bus_constants.h"
#include "who_am_i.h"

// void can_handler__driver_heartbeat_manage_mia(void (*handle_func)(dbc_DRIVER_HEARTBEAT_s));
void can_handler__driver_heartbeat_manage_mia(void);
void can_handler__driver_transmit(void);

void driver__process_motor_steering_values(void);
dbc_MOTOR_STEERING_s driver__process_motor_steering(void);

void driver__set_current_wheel_speed(dbc_MOTOR_SPEED_FEEDBACK_s);
dbc_MOTOR_SPEED_FEEDBACK_s driver__get_current_motor_wheel_speed_from_rpm_sensor(void);

void driver__set_sensor_input(dbc_SENSOR_SONARS_s);
dbc_MOTOR_SPEED_s driver__process_current_wheel_speed();
void driver__process_sensor_input(dbc_SENSOR_SONARS_s);

// dbc_DRIVER_HEARTBEAT_s get_dbc_DRIVER_HEARTBEAT_val(void);

void can_handler__receive_driver_heartbeat(dbc_message_header_t, can__msg_t);

void can_handler__motor_feedback_receive(dbc_message_header_t, can__msg_t);
