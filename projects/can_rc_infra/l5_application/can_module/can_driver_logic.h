#include "project.h"
#include <stdio.h>

void driver__process_motor_steering_values(void);
dbc_MOTOR_STEERING_s driver__process_motor_steering(void);

void driver__set_current_wheel_speed(dbc_MOTOR_SPEED_FEEDBACK_s);
dbc_MOTOR_SPEED_FEEDBACK_s driver__get_current_motor_wheel_speed_from_rpm_sensor(void);

void driver__set_sensor_input(dbc_SENSOR_SONARS_s);
dbc_MOTOR_SPEED_s driver__process_current_wheel_speed();
void driver__process_sensor_input(dbc_SENSOR_SONARS_s);
