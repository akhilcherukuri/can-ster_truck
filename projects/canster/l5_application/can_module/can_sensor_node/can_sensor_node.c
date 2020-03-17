#include "can_sensor_node.h"

#include "board_io.h"
#include "gpio.h"

void can_handler__sensor_heartbeat_manage_mia() {
#if BOARD_DRIVER_NODE == 1 || BOARD_MOTOR_NODE == 1
  const uint32_t mia_increment_value = 1000;
  static dbc_SENSOR_HEARTBEAT_s sensor_heartbeat = {{0}, 0};

  if (dbc_service_mia_SENSOR_HEARTBEAT(&sensor_heartbeat, mia_increment_value)) {
    fprintf(stderr, "MIA -> SENSOR_HEARTBEAT\r\n");
    fprintf(stderr, "assigned default sensor heartbeat = %d", sensor_heartbeat.SENSOR_HEARTBEAT_cmd);
    gpio__toggle(board_io__get_led2());
    // if (handle_func) {
    //   handle_func(sensor_heartbeat);
    // }
  }
#endif
}

void can_handler__sensor_transmit(void) {
#if BOARD_SENSOR_NODE == 1
  fprintf(stderr, "\nBOARD_SENSOR_NODE\n");

  dbc_SENSOR_HEARTBEAT_s message = {{0}, SENSOR_HEARTBEAT_cmd_SYNC};
  if (!dbc_encode_and_send_SENSOR_HEARTBEAT(NULL, &message)) {
#if DEBUG == 1
    fprintf(stderr, "\nFailed to encode and send Sensor Heartbeat\r\n");
#endif
  }

  dbc_SENSOR_SONARS_s sensor_sonar_data = {};
  sensor_sonar_data.SENSOR_SONARS_left = rand() % 500;
  sensor_sonar_data.SENSOR_SONARS_middle = rand() % 500;
  sensor_sonar_data.SENSOR_SONARS_right = rand() % 500;
  dbc_encode_and_send_SENSOR_SONARS(NULL, &sensor_sonar_data);
#endif
}

// dbc_SENSOR_HEARTBEAT_s get_dbc_SENSOR_HEARTBEAT_val(void) { return sensor_heartbeat; }

void can_handler__receive_sensor_heartbeat(dbc_message_header_t header, can__msg_t recv_message) {
  // dbc_SENSOR_HEARTBEAT_s sensor_heartbeat = get_dbc_SENSOR_HEARTBEAT_val();
  dbc_SENSOR_HEARTBEAT_s sensor_heartbeat_rcv;
  if (dbc_decode_SENSOR_HEARTBEAT(&sensor_heartbeat_rcv, header, recv_message.data.bytes)) {
#if DEBUG == 1
    fprintf(stderr, "\nSensor Heartbeat: %d\r\n", sensor_heartbeat_rcv.SENSOR_HEARTBEAT_cmd);
#endif
  }
}

void can_handler__sensor_sonar_receive(dbc_message_header_t header, can__msg_t recv_message) {
  dbc_SENSOR_SONARS_s sensor_sonar;
  if (dbc_decode_SENSOR_SONARS(&sensor_sonar, header, recv_message.data.bytes)) {
#if DEBUG == 1
    fprintf(stderr, "\nSensor values from SENSOR Node:\r\nLeft = %d\r\nRight = %d\r\nMiddle = %d\r\n",
            sensor_sonar.SENSOR_SONARS_left, sensor_sonar.SENSOR_SONARS_right, sensor_sonar.SENSOR_SONARS_middle);
#endif
    driver__set_sensor_input(sensor_sonar);
  }
}
