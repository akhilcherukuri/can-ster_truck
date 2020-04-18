#include "can_sensor_node.h"

#include "board_io.h"
#include "gpio.h"

#include "driver_obstacle.h"

#define SENSOR_NODE_DEBUG 1

#if SENSOR_NODE_DEBUG == 1
#include <stdio.h>
#endif

/**
 *
 * STATIC VARIABLES
 *
 */
static dbc_SENSOR_HEARTBEAT_s sensor_heartbeat;
static dbc_SENSOR_SONARS_s sensor_sonar;

/**
 * STATIC FUNCTIONS
 */
static void can_sensor__update_driver_obstacle(dbc_SENSOR_SONARS_s *sonar);

/**
 * Getters here
 */
const dbc_SENSOR_SONARS_s *can_sensor__get_sensor_sonar() { return &sensor_sonar; }
const dbc_SENSOR_HEARTBEAT_s *can_sensor__get_heartbeat() { return &sensor_heartbeat; }

/**
 * MIA
 */
void can_sensor__sensor_heartbeat_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_SENSOR_HEARTBEAT(&sensor_heartbeat, mia_increment_value)) {
#if SENSOR_NODE_DEBUG == 1
    printf("MIA -> SENSOR_HEARTBEAT\r\n");
    printf("assigned default sensor heartbeat = %d\r\n", sensor_heartbeat.SENSOR_HEARTBEAT_cmd);
#endif

    // Do other things here
    gpio__set(board_io__get_led3());
  }
}

void can_sensor__sensor_sonar_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_SENSOR_SONARS(&sensor_sonar, mia_increment_value)) {
#if SENSOR_NODE_DEBUG == 1
    printf("MIA -> SENSOR_SONAR\r\n");
    printf("assigned default sensor sonar values = %d:%d:%d\r\n", sensor_sonar.SENSOR_SONARS_left,
           sensor_sonar.SENSOR_SONARS_middle, sensor_sonar.SENSOR_SONARS_right);
#endif

    // DONE, Add more here
    // gpio__set(board_io__get_led2());
    can_sensor__update_driver_obstacle(&sensor_sonar);
  }
}

/**
 * TRANSMIT FUNCTIONS
 * #if BOARD_XYZ_NODE == 1
 *  -> logic
 * #else
 *  -> empty function
 * #endif
 */
#if BOARD_SENSOR_NODE == 1
static void can_sensor__transmit_sensor_heartbeat();
static void can_sensor__transmit_sensor_sonar();

void can_sensor__transmit_all_messages(void) {
  can_sensor__transmit_sensor_heartbeat();
  can_sensor__transmit_sensor_sonar();
}

static void can_sensor__transmit_sensor_heartbeat() {
  dbc_SENSOR_HEARTBEAT_s message = {{0}, SENSOR_HEARTBEAT_cmd_SYNC};
  if (!dbc_encode_and_send_SENSOR_HEARTBEAT(NULL, &message)) {
#if SENSOR_NODE_DEBUG == 1
    printf("Failed to encode and send Sensor Heartbeat\r\n");
#endif
  }
}

static void can_sensor__transmit_sensor_sonar() {
  dbc_SENSOR_SONARS_s sensor_sonar_data = {};
  sensor_sonar_data.SENSOR_SONARS_left = rand() % 500;
  sensor_sonar_data.SENSOR_SONARS_middle = rand() % 500;
  sensor_sonar_data.SENSOR_SONARS_right = rand() % 500;

  // TODO, Attach the ultrasonic hardware here

  if (!dbc_encode_and_send_SENSOR_SONARS(NULL, &sensor_sonar_data)) {
#if SENSOR_NODE_DEBUG == 1
    printf("Failed to encode and send Sensor Sonar\r\n");
#endif
  }
}

#else
void can_sensor__transmit_all_messages(void) {}
#endif

/**
 * DECODE FUNCTIONS
 */
void can_sensor__decode_sensor_heartbeat(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_SENSOR_HEARTBEAT(&sensor_heartbeat, header, bytes)) {
#if SENSOR_NODE_DEBUG == 1
    printf("\nSensor Heartbeat: %d\r\n", sensor_heartbeat.SENSOR_HEARTBEAT_cmd);
#endif

    // TODO, Do other things here
    // ! Add sensor heartbeat processing code here
    gpio__reset(board_io__get_led3());
  }
}

void can_sensor__decode_sensor_sonar(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_SENSOR_SONARS(&sensor_sonar, header, bytes)) {
#if SENSOR_NODE_DEBUG == 1
    printf("\nSensor values from SENSOR Node:\r\nLeft = %d\r\nRight = %d\r\nMiddle = %d\r\n",
           sensor_sonar.SENSOR_SONARS_left, sensor_sonar.SENSOR_SONARS_right, sensor_sonar.SENSOR_SONARS_middle);
#endif

    // TODO, Do other things here
    // ! Added sensor sonar processing code here
    can_sensor__update_driver_obstacle(&sensor_sonar);
  }
}

/**
 * STATIC FUNCTIONS
 */
static void can_sensor__update_driver_obstacle(dbc_SENSOR_SONARS_s *sonar) { driver_obstacle__process_input(sonar); }