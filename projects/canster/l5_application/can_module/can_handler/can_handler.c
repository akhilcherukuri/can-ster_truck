#include "can_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "can_bus_constants.h"

// TODO, Add all your node includes here
#include "can_driver_node.h"
#include "can_geo_node.h"
#include "can_motor_node.h"
#include "can_sensor_node.h"

/**
 * Defined functions
 */
void can_handler__handle_all_mia(void) {
  // Module function
  /**
   * NOTE, We will uncomment these functions depending on which node we are
   * And looking at the dbc file
   */

  // Senor Node MIA Functions
  // can_sensor__sensor_sonar_mia();
  // can_sensor__sensor_heartbeat_mia();

  // Driver Node MIA Functions
  // can_driver__motor_speed_mia();
  // can_driver__motor_steering_mia();
  can_driver__driver_heartbeat_mia();
  // can_driver__driver_coordinates_mia();

  // Motor Node MIA Functions
  can_motor__motor_heartbeat_mia();
  // can_motor__motor_speed_feedback_mia();

  // Geo Node MIA Functions
  // can_geo__geo_heartbeat_mia();
  // can_geo__geo_degree_mia();
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
    (void)header;

    /**
     * NOTE
     * We will uncomment these functions depending on which node we are
     * and the dbc file
     */

    // Sensor Node Decode functions
    // can_sensor__decode_sensor_heartbeat(header, recv_message.data.bytes);
    // can_sensor__decode_sensor_sonar(header, recv_message.data.bytes);

    // Driver Node Decode functions
    can_driver__decode_driver_heartbeat(header, recv_message.data.bytes);
    // can_driver__decode_motor_speed(header, recv_message.data.bytes);
    // can_driver__decode_motor_steering(header, recv_message.data.bytes);
    // can_driver__decode_driver_coordinates(header, recv_message.data.bytes);

    // Motor Node Decode functions
    can_motor__decode_motor_heartbeat(header, recv_message.data.bytes);
    // can_motor__decode_motor_speed_feedback(header, recv_message.data.bytes);

    // Geo Node Decode Functions
    // can_geo__decode_geo_heartbeat(header, recv_message.data.bytes);
    // can_geo__decode_geo_degree(header, recv_message.data.bytes);
  }
}

void can_handler__transmit_message_20hz(void) {
  // Module function

  // Sensor Node Transmit
  can_sensor__transmit_all_messages();

  // Driver Node Transmit
  can_driver__transmit_all_messages();

  // Motor Node Transmit
  can_motor__transmit_all_messages();

  // Geo Node Transmit
  can_geo__transmit_all_messages();
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
