#include "can_handler.h"

#include "can_bus_constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Defined functions
 */
void can_handler__handle_all_mia(void) {

  // Module function
  can_sensor__handle_all_mia();

  can_handler__driver_heartbeat_manage_mia();
  can_handler__motor_heartbeat_manage_mia();
}

void can_handler__handle_all_incoming_messages(void) {
  can__msg_t recv_message = {};

  while (can__rx(CAN_PORT, &recv_message, 0)) {
    const dbc_message_header_t header = {
        .message_id = recv_message.msg_id,
        .message_dlc = recv_message.frame_fields.data_len,
    };

#if RAW_DATA_DEBUG == 1
    fprintf(stderr, "Header Data: message_id: %lu | message_dlc: %d\r\n", header.message_id, header.message_dlc);
    fprintf(stderr, "Raw Data Words: %u %u %u\r\n", recv_message.data.words[0], recv_message.data.words[1],
            recv_message.data.words[2]);
#endif

    // Module function
    can_sensor__receive_all_messages(header, recv_message.data.bytes);

    // SIGNAL 100
    can_handler__receive_driver_heartbeat(header, recv_message);
    // SIGNAL 300
    can_handler__motor_heartbeat_receive(header, recv_message);
    // SIGNAL 500
    can_handler__motor_steering_receive(header, recv_message);
    // SIGNAL 600
    can_handler__motor_feedback_receive(header, recv_message);
    // SIGNAL 700
    can_handler__motor_receive(header, recv_message);
  }
}

void can_handler__transmit_message_10hz(void) {

  // Module function
  can_sensor__transmit_all_messages();

  can_handler__driver_transmit();

  can_handler__motor_transmit();
}

/**
 * Extern functions
 */
bool dbc_send_can_message(void *argument_from_dbc_encode_and_send, uint32_t message_id, const uint8_t bytes[8],
                          uint8_t dlc) {

  // unused
  (void)argument_from_dbc_encode_and_send;

  can__msg_t can_send = {};

  can_send.frame = 0;
  can_send.frame_fields.data_len = dlc;
  can_send.frame_fields.is_29bit = 0;
  can_send.frame_fields.is_rtr = 0;

  can_send.msg_id = message_id;
  memcpy(can_send.data.bytes, bytes, dlc);

  bool sent = true;
  if (!can__tx(CAN_PORT, &can_send, 0)) {
#if DEBUG == 1
    fprintf(stderr, "[%s] Can Tx Error\r\n", __FUNCTION__);
#endif
    sent = false;
  }
  return sent;
}
