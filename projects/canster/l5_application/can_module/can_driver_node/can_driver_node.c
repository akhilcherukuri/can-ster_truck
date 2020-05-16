#include <stdbool.h>

#include "board_io.h"
#include "gpio.h"

#include "can_driver_node.h"
#include "driver_obstacle.h"
#include "driver_state.h"
#include "geo_logic.h"
#include "lcd_ui.h"

#define DRIVER_NODE_DEBUG 0

#if DRIVER_NODE_DEBUG == 1
#include <stdio.h>
#endif

static dbc_MOTOR_STEERING_s driver_steering;
static dbc_MOTOR_SPEED_s driver_required_motor_speed;
static dbc_DRIVER_HEARTBEAT_s driver_heartbeat;
static dbc_DRIVER_COORDINATES_s driver_coordinates;

/**
 * GETTER FUNCTIONS
 */
dbc_MOTOR_STEERING_s can_driver__get_driver_steering() { return driver_steering; }
dbc_MOTOR_SPEED_s can_driver__get_driver_required_motor_speed() { return driver_required_motor_speed; }
dbc_DRIVER_HEARTBEAT_s can_driver__get_driver_heartbeat() { return driver_heartbeat; }
dbc_DRIVER_COORDINATES_s can_driver__get_driver_coordinates() { return driver_coordinates; }

/**
 * MIAs
 */
void can_driver__driver_heartbeat_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_DRIVER_HEARTBEAT(&driver_heartbeat, mia_increment_value)) {
#if DRIVER_NODE_DEBUG == 1
    printf("\nMIA -> DRIVER_HEARTBEAT");
    printf("\nAssigned default driver heartbeat = %d", driver_heartbeat.DRIVER_HEARTBEAT_cmd);
#endif
    set_lcd_mia_led(DRIVER_LED_MIA, false);
    gpio__set(board_io__get_led0());
  }
}

void can_driver__motor_speed_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_MOTOR_SPEED(&driver_required_motor_speed, mia_increment_value)) {
#if DRIVER_NODE_DEBUG == 1
    printf("\nMIA -> DRIVER_REQUIRED_MOTOR_SPEED");
    printf("\nAssigned default driver required motor speed = %lf",
           (double)driver_required_motor_speed.MOTOR_SPEED_processed);
#endif
  }
}

void can_driver__motor_steering_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_MOTOR_STEERING(&driver_steering, mia_increment_value)) {
#if DRIVER_NODE_DEBUG == 1
    printf("\nMIA -> DRIVER_STEERING");
    printf("\nAssigned default driver steering = %d", driver_steering.MOTOR_STEERING_direction);
#endif
  }
}

void can_driver__driver_coordinates_mia() {
  const uint32_t mia_increment_value = 1000;
  if (dbc_service_mia_DRIVER_COORDINATES(&driver_coordinates, mia_increment_value)) {
#if DRIVER_NODE_DEBUG == 1
    printf("\nMIA -> DRIVER_COORDINATES");
    printf("\nAssigned default driver coordinates - Lat: %lf, Long: %lf",
           (double)driver_coordinates.DRIVER_COORDINATES_latitude,
           (double)driver_coordinates.DRIVER_COORDINATES_longitude);
#endif
  }
}

/**
 * TRANSMITS
 */
#if BOARD_DRIVER_NODE == 1
static void can_driver__transmit_driver_steering(uint32_t);
static void can_driver__transmit_driver_required_motor_speed();
static void can_driver__transmit_driver_heartbeat();

void can_driver__transmit_all_messages(uint32_t callback_count) {
  can_driver__transmit_driver_steering(callback_count);
  can_driver__transmit_driver_required_motor_speed();
  can_driver__transmit_driver_heartbeat();
}

static void can_driver__transmit_driver_steering(uint32_t callback_count) {
  dbc_MOTOR_STEERING_s message = driver_obstacle__get_motor_commands(callback_count);

  if (!dbc_encode_and_send_MOTOR_STEERING(NULL, &message)) {
#if DRIVER_NODE_DEBUG == 1
    printf("\nFailed to encode and send Motor Steering");
#endif
  } else {
#if DRIVER_NODE_DEBUG == 1
    printf("\nSteering value sent = %d\n", message.MOTOR_STEERING_direction);
#endif
  }
}

static void can_driver__transmit_driver_required_motor_speed() {
  dbc_MOTOR_SPEED_s message;
  message.MOTOR_SPEED_processed = driver_obstacle__get_motor_speed_value();

  if (!dbc_encode_and_send_MOTOR_SPEED(NULL, &message)) {
#if DRIVER_NODE_DEBUG == 1
    printf("\nFailed to encode and send Motor Speed");
#endif
  }
}

static void can_driver__transmit_driver_heartbeat() {
  dbc_DRIVER_HEARTBEAT_s message = {{0}, DRIVER_HEARTBEAT_cmd_SYNC};
  if (!dbc_encode_and_send_DRIVER_HEARTBEAT(NULL, &message)) {
#if DRIVER_NODE_DEBUG == 1
    printf("\nFailed to encode and send Driver Heartbeat");
#endif
  }
}

#else
void can_driver__transmit_all_messages(void) {}
#endif

/**
 * DECODES
 */
static void can_driver__on_decode_driver_coordinates();

void can_driver__decode_driver_heartbeat(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_DRIVER_HEARTBEAT(&driver_heartbeat, header, bytes)) {
#if DRIVER_NODE_DEBUG == 1
    printf("\nDriver Heartbeat: %d", driver_heartbeat.DRIVER_HEARTBEAT_cmd);
#endif
    set_lcd_mia_led(DRIVER_LED_MIA, true);
    gpio__reset(board_io__get_led0());
  }
}

void can_driver__decode_motor_speed(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_MOTOR_SPEED(&driver_required_motor_speed, header, bytes)) {
#if DRIVER_NODE_DEBUG == 1
    printf("\nDriver Required motor speed: %lf", (double)driver_required_motor_speed.MOTOR_SPEED_processed);
#endif
  }
}

void can_driver__decode_motor_steering(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_MOTOR_STEERING(&driver_steering, header, bytes)) {
#if DRIVER_NODE_DEBUG == 1
    printf("\nDriver Required motor steering: %d", driver_steering.MOTOR_STEERING_direction);
#endif
  }
}

void can_driver__decode_driver_coordinates(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_DRIVER_COORDINATES(&driver_coordinates, header, bytes)) {
#if DRIVER_NODE_DEBUG == 1
    printf("\nDriver required driver coordinates: %lf %lf", (double)driver_coordinates.DRIVER_COORDINATES_latitude,
           (double)driver_coordinates.DRIVER_COORDINATES_longitude);
#endif
    can_driver__on_decode_driver_coordinates();
  }
}

static void can_driver__on_decode_driver_coordinates() {
  geo_logic__update_destination_coordinate(&driver_coordinates);
}
