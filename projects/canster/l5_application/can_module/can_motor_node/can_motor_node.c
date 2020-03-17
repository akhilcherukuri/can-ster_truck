#include "can_motor_node.h"
#include "can_driver_node.h"

static dbc_MOTOR_HEARTBEAT_s motor_heartbeat;

void can_handler__motor_heartbeat_manage_mia(void (*handle_func)(dbc_MOTOR_HEARTBEAT_s)) {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_MOTOR_HEARTBEAT(&motor_heartbeat, mia_increment_value)) {
    fprintf(stderr, "MIA -> MOTOR_HEARTBEAT\r\n");
    if (handle_func) {
      handle_func(motor_heartbeat);
    }
  }
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
#if DEBUG
    fprintf(stderr, "\nFailed to encode and send current Motor speed data to DRIVER Node\r\n");
#endif
  }
#endif
}

dbc_MOTOR_HEARTBEAT_s get_dbc_MOTOR_HEARTBEAT_val_from_driver_node_c_file(void) { return motor_heartbeat; }
