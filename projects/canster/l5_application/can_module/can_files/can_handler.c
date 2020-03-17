#include "can_handler.h"

#include "can_bus_constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "can_driver_node.h"
/**
 * Static variables declarations
 */
static dbc_SENSOR_SONARS_s sensor_sonar;
static dbc_MOTOR_STEERING_s motor_steering_val;
static dbc_MOTOR_SPEED_s motor_wheel_speed_val;
static dbc_MOTOR_SPEED_FEEDBACK_s motor_wheel_speed_current_val_from_rpm_sensor;

/**
 * Defined functions
 */

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
    // SIGNAL 100
    dbc_DRIVER_HEARTBEAT_s driver_heartbeat; //= get_dbc_DRIVER_HEARTBEAT_val();
    if (dbc_decode_DRIVER_HEARTBEAT(&driver_heartbeat, header, recv_message.data.bytes)) {
#if DEBUG == 1
      fprintf(stderr, "\nDriver Heartbeat: %d\r\n", driver_heartbeat.DRIVER_HEARTBEAT_cmd);
#endif
    }
    // SIGNAL 200
    // dbc_SENSOR_HEARTBEAT_s sensor_heartbeat = get_dbc_SENSOR_HEARTBEAT_val();
    dbc_SENSOR_HEARTBEAT_s sensor_heartbeat;
    if (dbc_decode_SENSOR_HEARTBEAT(&sensor_heartbeat, header, recv_message.data.bytes)) {
#if DEBUG == 1
      fprintf(stderr, "\nSensor Heartbeat: %d\r\n", sensor_heartbeat.SENSOR_HEARTBEAT_cmd);
#endif
    }
    // SIGNAL 300
    dbc_MOTOR_HEARTBEAT_s motor_heartbeat; // = get_dbc_MOTOR_HEARTBEAT_val();
    if (dbc_decode_MOTOR_HEARTBEAT(&motor_heartbeat, header, recv_message.data.bytes)) {
#if DEBUG == 1
      fprintf(stderr, "\nMotor Heartbeat: %d\r\n", motor_heartbeat.MOTOR_HEARTBEAT_cmd);
#endif
    }

    // SIGNAL 400
    if (dbc_decode_SENSOR_SONARS(&sensor_sonar, header, recv_message.data.bytes)) {
#if DEBUG == 1
      fprintf(stderr, "\nSensor values from SENSOR Node:\r\nLeft = %d\r\nRight = %d\r\nMiddle = %d\r\n",
              sensor_sonar.SENSOR_SONARS_left, sensor_sonar.SENSOR_SONARS_right, sensor_sonar.SENSOR_SONARS_middle);
#endif
      driver__set_sensor_input(sensor_sonar);
    }

    // SIGNAL 500
    if (dbc_decode_MOTOR_STEERING(&motor_steering_val, header, recv_message.data.bytes)) {
#if DEBUG == 1
      fprintf(stderr, "\nMotor Steering Value from MOTOR Node: %d\r\n", motor_steering_val.MOTOR_STEERING_direction);
#endif
    }

    // SIGNAL 600
    if (dbc_decode_MOTOR_SPEED(&motor_wheel_speed_val, header, recv_message.data.bytes)) {
#if DEBUG == 1
      fprintf(stderr, "\nCalculated Motor Wheelspeed Value from DRIVER Node: %lf\r\n",
              (double)motor_wheel_speed_val.MOTOR_SPEED_processed);
#endif
    }

    // SIGNAL 700
    /* Used only by DRIVER Node for the purpose of feedback monitoring */
    if (dbc_decode_MOTOR_SPEED_FEEDBACK(&motor_wheel_speed_current_val_from_rpm_sensor, header,
                                        recv_message.data.bytes)) {
#if DEBUG == 1
      fprintf(stderr, "\nCurrent Motor Wheel Speed Value from RPM Sensor via MOTOR Node: %lf\r\n",
              (double)motor_wheel_speed_current_val_from_rpm_sensor.MOTOR_SPEED_current);
#endif
      driver__set_current_wheel_speed(motor_wheel_speed_current_val_from_rpm_sensor);
    }
  }
}

void can_handler__transmit_message_10hz(void) {
  can_handler__sensor_transmit();

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
