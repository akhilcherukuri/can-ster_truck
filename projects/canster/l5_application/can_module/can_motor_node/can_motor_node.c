#include "can_motor_node.h"

#include "board_io.h"
#include "gpio.h"

#define DEBUG_MOTOR_NODE 1

#if DEBUG_MOTOR_NODE == 1
#include <stdio.h>
#endif

/**
 * STATIC VARIABLES
 */
static dbc_MOTOR_HEARTBEAT_s motor_heartbeat;
static dbc_MOTOR_SPEED_FEEDBACK_s motor_wheel_speed_current_val;

/**
 * GETTERS
 */
const dbc_MOTOR_HEARTBEAT_s *can_motor__get_heartbeat() { return &motor_heartbeat; }
const dbc_MOTOR_SPEED_FEEDBACK_s *can_motor__get_motor_speed_feedback() { return &motor_wheel_speed_current_val; }

#if BOARD_MOTOR_NODE == 1
/**
 * TRANSMIT
 */
static void can_motor__transmit_motor_heartbeat();
static void can_motor__transmit_motor_speed_feedback();

void can_motor__transmit_all_messages(void) {
  can_motor__transmit_motor_heartbeat();
  can_motor__transmit_motor_speed_feedback();
}

static void can_motor__transmit_motor_heartbeat() {
  dbc_MOTOR_HEARTBEAT_s message = {{0}, MOTOR_HEARTBEAT_cmd_SYNC};
  if (!dbc_encode_and_send_MOTOR_HEARTBEAT(NULL, &message)) {
#if DEBUG_MOTOR_NODE == 1
    printf("\nFailed to encode and send Motor Heartbeat");
#endif
  }
}

static void can_motor__transmit_motor_speed_feedback() {
  dbc_MOTOR_SPEED_FEEDBACK_s motor_feedback_speed_to_motor = {{0}, 60.0};

  // TODO, Attach the RPM Sensor hardware code here

  if (!dbc_encode_and_send_MOTOR_SPEED_FEEDBACK(NULL, &motor_feedback_speed_to_motor)) {
#if DEBUG_MOTOR_NODE == 1
    printf("\nFailed to encode and send current Motor speed data to DRIVER Node");
#endif
  }
}
#else
// Empty function
void can_motor__transmit_all_messages(void) {}

/**
 * MIA
 */
void can_motor__motor_heartbeat_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_MOTOR_HEARTBEAT(&motor_heartbeat, mia_increment_value)) {
#if DEBUG_MOTOR_NODE == 1
    printf("\nMIA -> MOTOR_HEARTBEAT");
    printf("\nAssigned default motor heartbeat = %d", motor_heartbeat.MOTOR_HEARTBEAT_cmd);
#endif
    gpio__set(board_io__get_led2());
  }
}

void can_motor__motor_speed_feedback_mia() {
  const uint32_t mia_increment_value = 1000;
  if (dbc_service_mia_MOTOR_SPEED_FEEDBACK(&motor_wheel_speed_current_val, mia_increment_value)) {
#if DEBUG_MOTOR_NODE == 1
    printf("\nMIA -> MOTOR_SPEED_FEEDBACK");
    printf("\nAssigned default motor speed for feedback = %lf",
           (double)motor_wheel_speed_current_val.MOTOR_SPEED_current);
#endif
  }
}

#endif

/**
 * DECODE FUNCTIONS
 */
void can_motor__decode_motor_heartbeat(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_MOTOR_HEARTBEAT(&motor_heartbeat, header, bytes)) {
#if DEBUG_MOTOR_NODE == 1
    printf("\nMotor Heartbeat: %d", motor_heartbeat.MOTOR_HEARTBEAT_cmd);
#endif
    gpio__reset(board_io__get_led2());
  }
}

/* Should be received only by DRIVER Node for the purpose of feedback monitoring */
void can_motor__decode_motor_speed_feedback(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_MOTOR_SPEED_FEEDBACK(&motor_wheel_speed_current_val, header, bytes)) {
#if DEBUG_MOTOR_NODE == 1
    printf("\nCurrent Motor Wheel Speed Value from RPM Sensor via MOTOR Node: %lf",
           (double)motor_wheel_speed_current_val.MOTOR_SPEED_current);
#endif
  }
}