#include "unity.h"

#include "Mockcan_bus.h"

#include "Mockcan_driver_node.h"
#include "Mockcan_geo_node.h"
#include "Mockcan_motor_node.h"
#include "Mockcan_sensor_node.h"

// Not Mocked
#include "can_bus_constants.h"
#include "can_handler.h"
#include "can_mia_configurations.c"

/**
 * Write your tests here
 * TODO, Add tests for each module added
 */
void setUp(void) {}
void tearDown(void) {}

void test_can_handler__handle_all_mia() {
  can_sensor__sensor_sonar_mia_Expect();
  can_sensor__sensor_heartbeat_mia_Expect();
  can_sensor__sensor_lidar_mia_Expect();
  can_motor__motor_heartbeat_mia_Expect();
  can_geo__geo_heartbeat_mia_Expect();
  can_geo__geo_degree_mia_Expect();
  can_geo__geo_destination_reached_mia_Expect();

  can_handler__handle_all_mia();
}

void test_can_handler__handle_all_incoming_messages(void) {

  can__msg_t recv_message = {};
  can__rx_ExpectAndReturn(CAN_PORT, &recv_message, 0, true);
  const dbc_message_header_t header = {
      .message_id = recv_message.msg_id,
      .message_dlc = recv_message.frame_fields.data_len,
  };

  // (void)header; // uncomment if used

  // SENSOR
  // can_sensor__decode_sensor_heartbeat_IgnoreArg_header();
  can_sensor__decode_sensor_heartbeat_ExpectAnyArgs();
  can_sensor__decode_sensor_sonar_ExpectAnyArgs();
  can_sensor__decode_sensor_lidar_ExpectAnyArgs();

  // MOTOR
  can_motor__decode_motor_heartbeat_ExpectAnyArgs();
  can_motor__decode_motor_speed_feedback_ExpectAnyArgs();

  // GEO
  can_geo__decode_geo_heartbeat_ExpectAnyArgs();
  can_geo__decode_geo_degree_ExpectAnyArgs();
  can_geo__decode_geo_destination_reached_ExpectAnyArgs();

  // TODO, Add more decode functions here as you build your node logic

  can__rx_ExpectAndReturn(CAN_PORT, &recv_message, 0, false);
  can_handler__handle_all_incoming_messages();
}

void test_can_handler__transmit_message_10hz(void) {
  can_sensor__transmit_all_messages_Expect();
  can_driver__transmit_all_messages_Expect();
  can_motor__transmit_all_messages_Expect();
  can_geo__transmit_all_messages_Expect();
  can_handler__transmit_message_10hz();
}

/**
 * EXTERN FUNCTION TEST
 */
void test_dbc_send_can_message(void) {
  can__msg_t message = {.data.bytes = {0x11, 0x22, 0x33, 0x44, 0, 0, 0, 0}};
  message.msg_id = 10;
  message.frame_fields.data_len = 4;
  can__tx_ExpectAndReturn(CAN_PORT, &message, 0, true);

  uint8_t message_id = 10;
  uint8_t bytes[8] = {0x11, 0x22, 0x33, 0x44, 0, 0, 0, 0};
  uint8_t dlc = 4;
  dbc_send_can_message(NULL, message_id, bytes, dlc);
}