
/**
 * TODO, Update
 * - `driver_state__update_destination_coordinate()` inside `can_sensor__on_decode`
 * -
 */

#include "can_driver_node.h"

#include "board_io.h"
#include "gpio.h"

#include "driver_obstacle.h"
#include "geo_logic.h"

#include "driver_state.h"

#define DRIVER_NODE_DEBUG 1

#if DRIVER_NODE_DEBUG == 1
#include <stdio.h>
#endif

static dbc_DRIVER_STEERING_s driver_steering;
static dbc_DRIVER_HEARTBEAT_s driver_heartbeat;

/**
 * TODO, Getter functions
 */
void can_driver__get_driver_steering(dbc_DRIVER_STEERING_s *return_steering) { *return_steering = driver_steering; }
void can_driver__get_driver_heartbeat(dbc_DRIVER_HEARTBEAT_s *return_heartbeat) {
  *return_heartbeat = driver_heartbeat;
}

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
    gpio__set(board_io__get_led0());
  }
}

void can_driver__driver_steering_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_DRIVER_STEERING(&driver_steering, mia_increment_value)) {
#if DRIVER_NODE_DEBUG == 1
    printf("MIA -> DRIVER_STEERING\r\n");
    printf("assigned default driver steering = %d, speed = %d\r\n", driver_steering.DRIVER_STEERING_direction,
           driver_steering.DRIVER_STEERING_speed);
#endif
  }
}

#if BOARD_DRIVER_NODE == 1

static void can_driver__transmit_driver_steering();
static void can_driver__transmit_driver_heartbeat();

void can_driver__transmit_all_messages(void) {
  can_driver__transmit_driver_heartbeat();
  can_driver__transmit_driver_steering();
}

static void can_driver__transmit_driver_steering() {
  dbc_DRIVER_STEERING_s message = driver_obstacle__get_motor_commands();

  if (!dbc_encode_and_send_DRIVER_STEERING(NULL, &message)) {
#if DRIVER_NODE_DEBUG == 1
    printf("Failed to encode and send Motor Steering Data\r\n");
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
    printf("\nDriver Heartbeat: %d\r\n", driver_heartbeat.DRIVER_HEARTBEAT_cmd);
#endif
    gpio__reset(board_io__get_led0());
  }
}

void can_driver__decode_driver_steering(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_DRIVER_STEERING(&driver_steering, header, bytes)) {
#if DRIVER_NODE_DEBUG == 1
    printf("Driver Required motor steering: %d, speed = %d\r\n", driver_steering.DRIVER_STEERING_direction,
           driver_steering.DRIVER_STEERING_speed);
#endif
  }
}
