#include "can_driver_node.h"

#include "board_io.h"
#include "gpio.h"

#include "driver_obstacle.h"

#define DRIVER_NODE_DEBUG 1

#if DRIVER_NODE_DEBUG == 1
#include <stdio.h>
#endif

static dbc_MOTOR_STEERING_s driver_steering;
static dbc_MOTOR_SPEED_s driver_required_motor_speed;
static dbc_DRIVER_HEARTBEAT_s driver_heartbeat;

/**
 * Getter functions
 */
const dbc_MOTOR_STEERING_s *can_driver__get_driver_steering() { return &driver_steering; }
const dbc_MOTOR_SPEED_s *can_driver__get_driver_required_motor_speed() { return &driver_required_motor_speed; }
const dbc_DRIVER_HEARTBEAT_s *can_driver__get_driver_heartbeat() { return &driver_heartbeat; }

/**
 * MIA
 */
void can_driver__driver_heartbeat_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_DRIVER_HEARTBEAT(&driver_heartbeat, mia_increment_value)) {
#if DRIVER_NODE_DEBUG == 1
    printf("MIA -> DRIVER_HEARTBEAT\r\n");
    printf("assigned default driver heartbeat = %d\r\n", driver_heartbeat.DRIVER_HEARTBEAT_cmd);
#endif
    gpio__set(board_io__get_led2());
  }
}

void can_driver__motor_speed_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_MOTOR_SPEED(&driver_required_motor_speed, mia_increment_value)) {
#if DRIVER_NODE_DEBUG == 1
    printf("MIA -> DRIVER_REQUIRED_MOTOR_SPEED\r\n");
    printf("assigned default driver required motor speed = %f\r\n",
           (double)driver_required_motor_speed.MOTOR_SPEED_processed);
#endif
    gpio__set(board_io__get_led2());
  }
}

void can_driver__motor_steering_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_MOTOR_STEERING(&driver_steering, mia_increment_value)) {
#if DRIVER_NODE_DEBUG == 1
    printf("MIA -> DRIVER_STEERING\r\n");
    printf("assigned default driver steering = %d\r\n", driver_steering.MOTOR_STEERING_direction);
#endif
    gpio__set(board_io__get_led2());
  }
}

#if BOARD_DRIVER_NODE == 1

static void can_driver__transmit_driver_steering();
static void can_driver__transmit_driver_required_motor_speed();
static void can_driver__transmit_driver_heartbeat();

void can_driver__transmit_all_messages(void) {
  can_driver__transmit_driver_steering();
  can_driver__transmit_driver_required_motor_speed();
  can_driver__transmit_driver_heartbeat();
}

static void can_driver__transmit_driver_steering() {
  dbc_MOTOR_STEERING_s message = driver_obstacle__get_motor_commands();

  if (!dbc_encode_and_send_MOTOR_STEERING(NULL, &message)) {
#if DRIVER_NODE_DEBUG == 1
    printf("Failed to encode and send Motor Steering Data\r\n");
#endif
  }
}

static void can_driver__transmit_driver_required_motor_speed() {

  // TODO, Get the required motor speed from hardware here
  // This will send the final reduced speed to the MOTOR Node
  dbc_MOTOR_SPEED_s message;
  message.MOTOR_SPEED_processed = 10.0;

  if (!dbc_encode_and_send_MOTOR_SPEED(NULL, &message)) {
#if DRIVER_NODE_DEBUG == 1
    printf("Failed to encode and send Motor Wheelspeed Data\r\n");
#endif
  }
}

static void can_driver__transmit_driver_heartbeat() {
  dbc_DRIVER_HEARTBEAT_s message = {{0}, DRIVER_HEARTBEAT_cmd_SYNC};
  if (!dbc_encode_and_send_DRIVER_HEARTBEAT(NULL, &message)) {
#if DRIVER_NODE_DEBUG == 1
    printf("Failed to encode and send Driver Heartbeat\r\n");
#endif
  }
}

#else
void can_driver__transmit_all_messages(void) {}
#endif

/**
 * DECODE
 */
void can_driver__decode_driver_heartbeat(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_DRIVER_HEARTBEAT(&driver_heartbeat, header, bytes)) {
#if DRIVER_NODE_DEBUG == 1
    printf("Driver Heartbeat: %d\r\n", driver_heartbeat.DRIVER_HEARTBEAT_cmd);
#endif

    // TODO, Do other things here
    // ! Make a function for process driver_heartbeat here
    gpio__reset(board_io__get_led2());
  }
}

void can_driver__decode_motor_speed(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_MOTOR_SPEED(&driver_required_motor_speed, header, bytes)) {
#if DRIVER_NODE_DEBUG == 1
    printf("Driver Required motor speed: %f\r\n", (double)driver_required_motor_speed.MOTOR_SPEED_processed);
#endif

    // TODO, Do other things here
    // ! Make a function for process required_motor_speed here
    gpio__reset(board_io__get_led2());
  }
}

void can_driver__decode_motor_steering(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_MOTOR_STEERING(&driver_steering, header, bytes)) {
#if DRIVER_NODE_DEBUG == 1
    printf("Driver Required motor steering: %d\r\n", driver_steering.MOTOR_STEERING_direction);
#endif

    // TODO, Do other things here
    // ! Make a function for process motor_steering here
    gpio__reset(board_io__get_led2());
  }
}
