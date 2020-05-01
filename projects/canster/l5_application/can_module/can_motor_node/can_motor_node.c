#include "can_motor_node.h"

#include "board_io.h"
#include "gpio.h"

#define DEBUG_MOTOR_NODE 1

#if DEBUG_MOTOR_NODE == 1
#include <stdio.h>
#endif

/**
 *
 * STATIC VARIABLES
 *
 */
static dbc_MOTOR_HEARTBEAT_s motor_heartbeat;
static dbc_MOTOR_INFO_s motor_info;
static dbc_MOTOR_DEBUG_s motor_debug;

/**
 * DONE, Getters here
 */
void can_motor__get_heartbeat(dbc_MOTOR_HEARTBEAT_s *return_heartbeat) { *return_heartbeat = motor_heartbeat; }
void can_motor__get_motor_info(dbc_MOTOR_INFO_s *return_info) { *return_info = motor_info; }
void can_motor__get_motor_debug(dbc_MOTOR_DEBUG_s *return_debug) { *return_debug = motor_debug; }

#if BOARD_MOTOR_NODE == 1
static void can_motor__transmit_motor_heartbeat();
static void can_motor__transmit_motor_info();

static void can_motor__transmit_motor_debug();

void can_motor__transmit_all_messages(void) {
  can_motor__transmit_motor_heartbeat();
  can_motor__transmit_motor_info();

  can_motor__transmit_motor_debug();
}

static void can_motor__transmit_motor_heartbeat() {
  dbc_MOTOR_HEARTBEAT_s message = {{0}, MOTOR_HEARTBEAT_cmd_SYNC};
  if (!dbc_encode_and_send_MOTOR_HEARTBEAT(NULL, &message)) {
#if DEBUG_MOTOR_NODE == 1
    printf("Failed to encode and send Motor Heartbeat\r\n");
#endif
  }
}

static void can_motor__transmit_motor_info() {
  dbc_MOTOR_INFO_s message_motor_info = {};

  // TODO, Attach the RPM Sensor hardware code here

  if (!dbc_encode_and_send_MOTOR_INFO(NULL, &message_motor_info)) {
#if DEBUG_MOTOR_NODE == 1
    printf("Failed to encode and send current Motor speed data to DRIVER Node\r\n");
#endif
  }
}

static void can_motor__transmit_motor_debug() {
  dbc_MOTOR_DEBUG_s message_debug = {};
  // TODO, Update the message here

  if (!dbc_encode_and_send_MOTOR_DEBUG(NULL, &message_debug)) {
#if DEBUG_MOTOR_NODE == 1
    printf("Failed to encode and send Motor Debug message\r\n");
#endif
  }
}

#else
void can_motor__transmit_all_messages(void) {}
#endif

/**
 * MIA is applicable only when BOARD_MOTOR_NODE != 1
 */
void can_motor__motor_heartbeat_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_MOTOR_HEARTBEAT(&motor_heartbeat, mia_increment_value)) {
#if DEBUG_MOTOR_NODE == 1
    printf("MIA -> MOTOR_HEARTBEAT\r\n");
    printf("assigned default motor heartbeat = %d\r\n", motor_heartbeat.MOTOR_HEARTBEAT_cmd);
#endif
    gpio__set(board_io__get_led1());
  }
}

void can_motor__motor_info_mia() {
  const uint32_t mia_increment_value = 1000;
  if (dbc_service_mia_MOTOR_INFO(&motor_info, mia_increment_value)) {
#if DEBUG_MOTOR_NODE == 1
    printf("MIA -> MOTOR_SPEED_FEEDBACK\r\n");
#endif
    printf("assigned default motor speed for feedback rps = %f, kph = %f\r\n", (double)motor_info.MOTOR_INFO_rps,
           (double)motor_info.MOTOR_INFO_kph);
    // gpio__set(board_io__get_led1());
  }
}

/**
 * DECODE FUNCTIONS
 */
void can_motor__decode_motor_heartbeat(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_MOTOR_HEARTBEAT(&motor_heartbeat, header, bytes)) {
#if DEBUG_MOTOR_NODE == 1
    printf("Motor Heartbeat: %d\r\n", motor_heartbeat.MOTOR_HEARTBEAT_cmd);
#endif
    gpio__reset(board_io__get_led1());
  }
}

/* Should be received only by DRIVER Node for the purpose of feedback monitoring */
void can_motor__decode_motor_info(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_MOTOR_INFO(&motor_info, header, bytes)) {
#if DEBUG_MOTOR_NODE == 1
    printf("Current Motor Wheel Speed Value from RPM Sensor via MOTOR Node: rps = %lf, kph = %f\r\n",
           (double)motor_info.MOTOR_INFO_rps, (double)motor_info.MOTOR_INFO_kph);
#endif
  }
}

void can_motor__decode_motor_debug(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_MOTOR_DEBUG(&motor_debug, header, bytes)) {
#if DEBUG_MOTOR_NODE == 1
    printf("Current Motor Debug: Lipo Voltage %f Pwm Duty Cycle %f\r\n", (double)motor_debug.MOTOR_DEBUG_lipo_voltage,
           (double)motor_debug.MOTOR_DEBUG_pwm_duty_cycle);
#endif
  }
}
