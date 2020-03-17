#include "can_sensor_node.h"

static dbc_SENSOR_HEARTBEAT_s sensor_heartbeat;

void can_handler__sensor_heartbeat_manage_mia(void (*handle_func)(dbc_SENSOR_HEARTBEAT_s)) {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_SENSOR_HEARTBEAT(&sensor_heartbeat, mia_increment_value)) {
    fprintf(stderr, "MIA -> SENSOR_HEARTBEAT\r\n");
    if (handle_func) {
      handle_func(sensor_heartbeat);
    }
  }
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

dbc_SENSOR_HEARTBEAT_s get_dbc_SENSOR_HEARTBEAT_val(void) { return sensor_heartbeat; }
