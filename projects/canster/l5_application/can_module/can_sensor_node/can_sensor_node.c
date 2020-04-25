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

static dbc_SENSOR_BT_COORDINATES_s sensor_bt_coordinates;
static dbc_SENSOR_LIDAR_s sensor_lidar;

/**
 * STATIC FUNCTIONS
 */
static void can_sensor__update_driver_obstacle(dbc_SENSOR_SONARS_s *sonar);

/**
 * Getters here
 */
void can_sensor__get_heartbeat(dbc_SENSOR_HEARTBEAT_s *return_heartbeat) { *return_heartbeat = sensor_heartbeat; }

void can_sensor__get_sensor_sonar(dbc_SENSOR_SONARS_s *return_sonar) { *return_sonar = sensor_sonar; }

void can_sensor__get_sensor_bt_coordinates(dbc_SENSOR_BT_COORDINATES_s *return_bt_coordinates) {
  *return_bt_coordinates = sensor_bt_coordinates;
}

void can_sensor__get_sensor_lidar(dbc_SENSOR_LIDAR_s *return_lidar) { *return_lidar = sensor_lidar; }

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
    printf("assigned default sensor sonar values = %f:%f:%f\r\n", (double)sensor_sonar.SENSOR_SONARS_left,
           (double)sensor_sonar.SENSOR_SONARS_middle, (double)sensor_sonar.SENSOR_SONARS_right);
#endif

    // DONE, Add more here
    // gpio__set(board_io__get_led2());
    can_sensor__update_driver_obstacle(&sensor_sonar);
  }
}

void can_sensor__sensor_bt_coordinates_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_SENSOR_BT_COORDINATES(&sensor_bt_coordinates, mia_increment_value)) {
#if SENSOR_NODE_DEBUG == 1
    printf("MIA -> SENSOR_BT_COORDINATES\r\n");
#endif

    // TODO, Do something here
  }
}

void can_sensor__sensor_lidar_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_SENSOR_LIDAR(&sensor_lidar, mia_increment_value)) {
#if SENSOR_NODE_DEBUG == 1
    printf("MIA -> SENSOR_LIDAR\r\n");
#endif

    // TODO, Do something here
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
static void can_sensor__transmit_sensor_bt_coordinates();
static void can_sensor__transmit_sensor_lidar();

void can_sensor__transmit_all_messages(void) {
  can_sensor__transmit_sensor_heartbeat();
  can_sensor__transmit_sensor_sonar();
  can_sensor__transmit_sensor_bt_coordinates();
  can_sensor__transmit_sensor_lidar();
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

static void can_sensor__transmit_sensor_bt_coordinates() {
  dbc_SENSOR_BT_COORDINATES_s coordinates = {};
  // TODO, Create the message here

  if (!dbc_encode_and_send_SENSOR_BT_COORDINATES(NULL, &coordinates)) {
#if SENSOR_NODE_DEBUG == 1
    printf("Failed to encode and send Sensor BT Coordinates\r\n");
#endif
  }
}

static void can_sensor__transmit_sensor_lidar() {
  dbc_SENSOR_LIDAR_s lidar = {};
  // TODO, Create the message here

  if (!dbc_encode_and_send_SENSOR_LIDAR(NULL, &lidar)) {
#if SENSOR_NODE_DEBUG == 1
    printf("Failed to encode and send Sensor Lidar\r\n");
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
    printf("\nSensor values from SENSOR Node:\r\nLeft = %f\r\nRight = %f\r\nMiddle = %f\r\n",
           (double)sensor_sonar.SENSOR_SONARS_left, (double)sensor_sonar.SENSOR_SONARS_right,
           (double)sensor_sonar.SENSOR_SONARS_middle);
#endif

    // TODO, Do other things here
    // ! Added sensor sonar processing code here
    can_sensor__update_driver_obstacle(&sensor_sonar);
  }
}

void can_sensor__decode_sensor_bt_coordinates(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_SENSOR_BT_COORDINATES(&sensor_bt_coordinates, header, bytes)) {
#if SENSOR_NODE_DEBUG == 1
    printf("Sensor Values for BT Coordinates: %f %f\r\n", (double)sensor_bt_coordinates.SENSOR_BT_COORDINATES_latitude,
           (double)sensor_bt_coordinates.SENSOR_BT_COORDINATES_longitude);
#endif

    // TODO, Do other things here
  }
}

void can_sensor__decode_sensor_lidar(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_SENSOR_LIDAR(&sensor_lidar, header, bytes)) {
#if SENSOR_NODE_DEBUG == 1
    printf("Sensor Values for Lidar: %d %d %d %d\r\n", sensor_lidar.SENSOR_LIDAR_back, sensor_lidar.SENSOR_LIDAR_middle,
           sensor_lidar.SENSOR_LIDAR_slight_left, sensor_lidar.SENSOR_LIDAR_slight_right);
#endif

    // TODO, Do other things here
  }
}

/**
 * STATIC FUNCTIONS
 */
static void can_sensor__update_driver_obstacle(dbc_SENSOR_SONARS_s *sonar) { driver_obstacle__process_input(sonar); }