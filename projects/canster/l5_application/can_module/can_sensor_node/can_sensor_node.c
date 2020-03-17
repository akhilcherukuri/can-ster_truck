#include "can_sensor_node.h"

#include "board_io.h"
#include "gpio.h"

/**
 *
 * STATIC VARIABLES
 *
 */
static dbc_SENSOR_HEARTBEAT_s sensor_heartbeat = {{0}, 0};

/**
 * START
 */
static void can_sensor__sensor_heartbeat_mia();
static void can_sensor__sensor_sonar_mia();

void can_sensor__handle_all_mia() {
  can_sensor__sensor_heartbeat_mia();
  can_sensor__sensor_sonar_mia();
}

static void can_sensor__sensor_heartbeat_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_SENSOR_HEARTBEAT(&sensor_heartbeat, mia_increment_value)) {
    printf("MIA -> SENSOR_HEARTBEAT\r\n");
    printf("assigned default sensor heartbeat = %d", sensor_heartbeat.SENSOR_HEARTBEAT_cmd);
    gpio__toggle(board_io__get_led2());
  }
}
// TODO, Write this function
static void can_sensor__sensor_sonar_mia() {}

#if BOARD_SENSOR_NODE == 1
static can_sensor__transmit_sensor_heartbeat();
static can_sensor__transmit_sensor_sonar();

void can_sensor__transmit_all_messages(void) {
  can_sensor__transmit_sensor_heartbeat();
  can_sensor__transmit_sensor_sonar();
}

static can_sensor__transmit_sensor_heartbeat() {
  dbc_SENSOR_HEARTBEAT_s message = {{0}, SENSOR_HEARTBEAT_cmd_SYNC};
  if (!dbc_encode_and_send_SENSOR_HEARTBEAT(NULL, &message)) {
#if DEBUG == 1
    printf("\nFailed to encode and send Sensor Heartbeat\r\n");
#endif
  }
}

static can_sensor__transmit_sensor_sonar() {
  dbc_SENSOR_SONARS_s sensor_sonar_data = {};
  sensor_sonar_data.SENSOR_SONARS_left = rand() % 500;
  sensor_sonar_data.SENSOR_SONARS_middle = rand() % 500;
  sensor_sonar_data.SENSOR_SONARS_right = rand() % 500;
  dbc_encode_and_send_SENSOR_SONARS(NULL, &sensor_sonar_data);
}
#else
void can_sensor__transmit_all_messages(void) {}
#endif

static void can_sensor__decode_sensor_heartbeat(dbc_message_header_t header, uint8_t bytes[8]);
static void can_sensor__decode_sensor_sonar(dbc_message_header_t header, uint8_t bytes[8]);

void can_sensor__receive_all_messages(dbc_message_header_t header, uint8_t bytes[8]) {
  can_sensor__decode_sensor_heartbeat(header, bytes);
  can_sensor__decode_sensor_sonar(header, bytes);
}

static void can_sensor__decode_sensor_heartbeat(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_SENSOR_HEARTBEAT(&sensor_heartbeat, header, bytes)) {
#if DEBUG == 1
    printf("\nSensor Heartbeat: %d\r\n", sensor_heartbeat.SENSOR_HEARTBEAT_cmd);
#endif
  }
}

static void can_sensor__decode_sensor_sonar(dbc_message_header_t header, uint8_t bytes[8]) {
  dbc_SENSOR_SONARS_s sensor_sonar;
  if (dbc_decode_SENSOR_SONARS(&sensor_sonar, header, bytes)) {
#if DEBUG == 1
    printf("\nSensor values from SENSOR Node:\r\nLeft = %d\r\nRight = %d\r\nMiddle = %d\r\n",
           sensor_sonar.SENSOR_SONARS_left, sensor_sonar.SENSOR_SONARS_right, sensor_sonar.SENSOR_SONARS_middle);
#endif
    driver__set_sensor_input(sensor_sonar);
  }
}