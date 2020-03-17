#include "can_motor_node.h"
#include "can_driver_node.h"

#include "board_io.h"
#include "gpio.h"

void can_handler__motor_heartbeat_manage_mia() {
#if BOARD_DRIVER_NODE == 1 || BOARD_SENSOR_NODE == 1

  const uint32_t mia_increment_value = 1000;
  static dbc_MOTOR_HEARTBEAT_s motor_heartbeat;
  if (dbc_service_mia_MOTOR_HEARTBEAT(&motor_heartbeat, mia_increment_value)) {

    fprintf(stderr, "MIA -> MOTOR_HEARTBEAT\r\n");
    // if (handle_func) {
    //   handle_func(motor_heartbeat);
    fprintf(stderr, "assigned default motor heartbeat = %d", motor_heartbeat.MOTOR_HEARTBEAT_cmd);
    gpio__toggle(board_io__get_led1());
  }
  //  }
#endif
}

void can_handler__motor_transmit(void) {
#if BOARD_MOTOR_NODE == 1
  fprintf(stderr, "\nBOARD_MOTOR_NODE\n");

  dbc_MOTOR_HEARTBEAT_s message = {{0}, MOTOR_HEARTBEAT_cmd_SYNC};
  if (!dbc_encode_and_send_MOTOR_HEARTBEAT(NULL, &message)) {
#if DEBUG == 1
    fprintf(stderr, "\nFailed to encode and send Motor Heartbeat\r\n");
#endif
  }

  dbc_MOTOR_SPEED_FEEDBACK_s motor_wheel_speed_current_val = driver__get_current_motor_wheel_speed_from_rpm_sensor();
  if (!dbc_encode_and_send_MOTOR_SPEED_FEEDBACK(NULL, &motor_wheel_speed_current_val)) {
#if DEBUG == 1
    fprintf(stderr, "\nFailed to encode and send current Motor speed data to DRIVER Node\r\n");
#endif
  }
#endif
}

// dbc_MOTOR_HEARTBEAT_s get_dbc_MOTOR_HEARTBEAT_val(void) { return motor_heartbeat; }

void can_handler__motor_heartbeat_receive(dbc_message_header_t header, can__msg_t recv_message) {
  // dbc_MOTOR_HEARTBEAT_s motor_heartbeat = get_dbc_MOTOR_HEARTBEAT_val();
  dbc_MOTOR_HEARTBEAT_s motor_heartbeat;
  if (dbc_decode_MOTOR_HEARTBEAT(&motor_heartbeat, header, recv_message.data.bytes)) {
#if DEBUG == 1
    fprintf(stderr, "\nMotor Heartbeat: %d\r\n", motor_heartbeat.MOTOR_HEARTBEAT_cmd);
#endif
  }
}

void can_handler__motor_steering_receive(dbc_message_header_t header, can__msg_t recv_message) {
  dbc_MOTOR_STEERING_s motor_steering_val;
  if (dbc_decode_MOTOR_STEERING(&motor_steering_val, header, recv_message.data.bytes)) {
#if DEBUG == 1
    fprintf(stderr, "\nMotor Steering Value from MOTOR Node: %d\r\n", motor_steering_val.MOTOR_STEERING_direction);
#endif
  }
}

void can_handler__motor_receive(dbc_message_header_t header, can__msg_t recv_message) {
  dbc_MOTOR_SPEED_s motor_wheel_speed_val;
  if (dbc_decode_MOTOR_SPEED(&motor_wheel_speed_val, header, recv_message.data.bytes)) {
#if DEBUG == 1
    fprintf(stderr, "\nCalculated Motor Wheelspeed Value from DRIVER Node: %lf\r\n",
            (double)motor_wheel_speed_val.MOTOR_SPEED_processed);
#endif
  }
}
