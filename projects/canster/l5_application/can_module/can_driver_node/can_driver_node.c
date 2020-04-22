
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

static dbc_MOTOR_STEERING_s driver_steering;
static dbc_MOTOR_SPEED_s driver_required_motor_speed;
static dbc_DRIVER_HEARTBEAT_s driver_heartbeat;
static dbc_DRIVER_COORDINATES_s driver_coordinates;

/**
 * Getter functions
 */
const dbc_MOTOR_STEERING_s *can_driver__get_driver_steering() { return &driver_steering; }
const dbc_MOTOR_SPEED_s *can_driver__get_driver_required_motor_speed() { return &driver_required_motor_speed; }
const dbc_DRIVER_HEARTBEAT_s *can_driver__get_driver_heartbeat() { return &driver_heartbeat; }
const dbc_DRIVER_COORDINATES_s *can_driver__get_driver_coordinates() { return &driver_coordinates; }
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
    // gpio__set(board_io__get_led2());
  }
}

void can_driver__motor_steering_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_MOTOR_STEERING(&driver_steering, mia_increment_value)) {
#if DRIVER_NODE_DEBUG == 1
    printf("MIA -> DRIVER_STEERING\r\n");
    printf("assigned default driver steering = %d\r\n", driver_steering.MOTOR_STEERING_direction);
#endif
    // gpio__set(board_io__get_led2());
  }
}

void can_driver__driver_coordinates_mia() {
  const uint32_t mia_increment_value = 1000;
  if (dbc_service_mia_DRIVER_COORDINATES(&driver_coordinates, mia_increment_value)) {
#if DRIVER_NODE_DEBUG == 1
    printf("MIA -> DRIVER_COORDINATES\r\n");
    printf("assigned default driver coordinates = %f %f\r\n", (double)driver_coordinates.DRIVER_COORDINATES_latitude,
           (double)driver_coordinates.DRIVER_COORDINATES_longitude);
#endif
  }
}

#if BOARD_DRIVER_NODE == 1

static void can_driver__transmit_driver_steering();
static void can_driver__transmit_driver_required_motor_speed();
static void can_driver__transmit_driver_heartbeat();
static void can_driver__transmit_driver_coordinates();

void can_driver__transmit_all_messages(void) {
  can_driver__transmit_driver_steering();
  can_driver__transmit_driver_required_motor_speed();
  can_driver__transmit_driver_heartbeat();
  // can_driver__transmit_driver_coordinates();
}

static void can_driver__transmit_driver_coordinates() {

  const dbc_DRIVER_COORDINATES_s *destination_coordinate = driver_state__get_destination_coordinate();
  dbc_DRIVER_COORDINATES_s message = *destination_coordinate;

  if (!dbc_encode_and_send_DRIVER_COORDINATES(NULL, &message)) {
#if DRIVER_NODE_DEBUG == 1
    printf("Failed to encode and send Driver Coordinates\r\n");
#endif
  }
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
  message.MOTOR_SPEED_processed = speed_value;

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
static void can_driver__on_decode_driver_coordinates();

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
    // gpio__reset(board_io__get_led2());
  }
}

void can_driver__decode_motor_steering(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_MOTOR_STEERING(&driver_steering, header, bytes)) {
#if DRIVER_NODE_DEBUG == 1
    printf("Driver Required motor steering: %d\r\n", driver_steering.MOTOR_STEERING_direction);
#endif

    // TODO, Do other things here
    // ! Make a function for process motor_steering here
    // gpio__reset(board_io__get_led2());
  }
}

void can_driver__decode_driver_coordinates(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_DRIVER_COORDINATES(&driver_coordinates, header, bytes)) {
#if DRIVER_NODE_DEBUG == 1
    printf("Driver required driver coordinates: %f %f\r\n", (double)driver_coordinates.DRIVER_COORDINATES_latitude,
           (double)driver_coordinates.DRIVER_COORDINATES_longitude);
#endif

    // DONE, Add other things here
    can_driver__on_decode_driver_coordinates();
  }
}

static void can_driver__on_decode_driver_coordinates() {
  // DONE, Update the geo logic/state here
  geo_logic__update_destination_coordinate(&driver_coordinates);
}
