#include "periodic_callbacks.h"

#include "board_io.h"
#include "gpio.h"

#include "can_bus_initializer.h"

#include "can_handler.h"
#include "project.h"

#include <stdio.h>

/**
 * Static functions declaration
 */

// static void driver_mia_handle_func(dbc_DRIVER_HEARTBEAT_s driver_heartbeat);
// static void sensor_mia_handle_func(dbc_SENSOR_HEARTBEAT_s sensor_heartbeat);
// static void motor_mia_handle_func(dbc_MOTOR_HEARTBEAT_s motor_heartbeat);
/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */
void periodic_callbacks__initialize(void) {
  // This method is invoked once when the periodic tasks are created
  can_bus__initialize();
}

void periodic_callbacks__1Hz(uint32_t callback_count) { can_handler__handle_all_mia(); }

void periodic_callbacks__10Hz(uint32_t callback_count) {
  can_handler__transmit_message_10hz();
  can_handler__handle_all_incoming_messages();
}

void periodic_callbacks__100Hz(uint32_t callback_count) {}

void periodic_callbacks__1000Hz(uint32_t callback_count) {}

/**
 * STATIC FUNCTION
 */

// Set the MIA LED callbacks
// static void driver_mia_handle_func(dbc_DRIVER_HEARTBEAT_s driver_heartbeat) {
//   fprintf(stderr, "MIA Driver Default Value: %d\r\n", driver_heartbeat.DRIVER_HEARTBEAT_cmd);
//   gpio__toggle(board_io__get_led2());
// }

// static void sensor_mia_handle_func(dbc_SENSOR_HEARTBEAT_s sensor_heartbeat) {
//   fprintf(stderr, "MIA Sensor Default Value: %d\r\n", sensor_heartbeat.SENSOR_HEARTBEAT_cmd);
//   gpio__toggle(board_io__get_led3());
// }

// static void motor_mia_handle_func(dbc_MOTOR_HEARTBEAT_s motor_heartbeat) {
//   fprintf(stderr, "MIA Motor Default Value: %d\r\n", motor_heartbeat.MOTOR_HEARTBEAT_cmd);
//   gpio__toggle(board_io__get_led1());
// }