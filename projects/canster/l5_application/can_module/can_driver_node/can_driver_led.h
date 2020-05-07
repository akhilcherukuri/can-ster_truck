#pragma once

void can_driver_led__init();

// Set
void can_driver_led__set_motor_heartbeat_mia();
void can_driver_led__set_geo_heartbeat_mia();
void can_driver_led__set_sensor_heartbeat_mia();

// Reset
void can_driver_led__reset_motor_heartbeat_mia();
void can_driver_led__reset_geo_heartbeat_mia();
void can_driver_led__reset_sensor_heartbeat_mia();
