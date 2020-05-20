#include "can_sensor_node.h"
#include "project.h"

#include "board_io.h"
#include "gpio.h"

#include "bt_wrapper.h"
#include "driver_obstacle.h"
#include "gps.h"
#include "lidar_data_handler.h"
#include "ultrasonic_wrapper.h"

#define SENSOR_NODE_DEBUG 0

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
static dbc_SENSOR_LIDAR_s sensor_lidar;

/**
 * STATIC FUNCTIONS
 */
static void can_sensor__update_driver_obstacle(dbc_SENSOR_SONARS_s *);

/**
 * Getters here
 */
const dbc_SENSOR_SONARS_s *can_sensor__get_sensor_sonar() { return &sensor_sonar; }
const dbc_SENSOR_HEARTBEAT_s *can_sensor__get_heartbeat() { return &sensor_heartbeat; }
const dbc_SENSOR_LIDAR_s *can_sensor__get_sensor_lidar() { return &sensor_lidar; }

/**
 * MIA
 */
void can_sensor__sensor_heartbeat_mia() {
  const uint32_t mia_increment_value = 100;

  if (dbc_service_mia_SENSOR_HEARTBEAT(&sensor_heartbeat, mia_increment_value)) {
#if SENSOR_NODE_DEBUG == 1
    printf("MIA -> SENSOR_HEARTBEAT\r\n");
    printf("Assigned default sensor heartbeat = %d\r\n", sensor_heartbeat.SENSOR_HEARTBEAT_cmd);
#endif

    // TODO: Set external MIA LED
    gpio__set(board_io__get_led3());
  }
}

void can_sensor__sensor_sonar_mia() {
  const uint32_t mia_increment_value = 100;

  if (dbc_service_mia_SENSOR_SONARS(&sensor_sonar, mia_increment_value)) {
#if SENSOR_NODE_DEBUG == 1
    printf("MIA -> SENSOR_SONAR\r\n");
    printf("Assigned default sensor sonar values = L: %lf, M: %lf, R: %lf\r\n", (double)sensor_sonar.SENSOR_SONARS_left,
           (double)sensor_sonar.SENSOR_SONARS_middle, (double)sensor_sonar.SENSOR_SONARS_right);
#endif

    // TODO: Set external MIA LED
    gpio__set(board_io__get_led2());
    can_sensor__update_driver_obstacle(&sensor_sonar);
  }
}

void can_sensor__sensor_lidar_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_SENSOR_LIDAR(&sensor_lidar, mia_increment_value)) {
#if SENSOR_NODE_DEBUG == 1
    printf("MIA -> SENSOR_LIDAR\r\n");
    printf("\nAssigned default sensor sonar values = \r\nLeft: %d\r\nRight: %d\r\nFront: %d\r\nRear: %d\r\n",
           sensor_lidar.SENSOR_LIDAR_slight_left, sensor_lidar.SENSOR_LIDAR_slight_right,
           sensor_lidar.SENSOR_LIDAR_middle, sensor_lidar.SENSOR_LIDAR_back);
#endif

    // TODO: Set external MIA LED
    // gpio_set(board_io_get_led2());
    // can_sensor__update_driver_obstacle(&sensor_sonar);
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
static void can_sensor__transmit_sensor_lidar();
static void can_sensor__transmit_sensor_bt_coordinates();
static void can_sensor__transmit_motor_key();

void can_sensor__transmit_all_messages(void) {
  can_sensor__transmit_sensor_heartbeat();
  can_sensor__transmit_sensor_sonar();
  can_sensor__transmit_sensor_lidar();
  can_sensor__transmit_sensor_bt_coordinates();
  can_sensor__transmit_motor_key();
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
  ultrasonic__update_all_sensors();
  ultrasonic__get_distance_from_all_sensors(&sensor_sonar_data);

  if (!dbc_encode_and_send_SENSOR_SONARS(NULL, &sensor_sonar_data)) {
#if SENSOR_NODE_DEBUG == 1
    printf("Failed to encode and send Sensor Sonar\r\n");
#endif
  }
}

static void can_sensor__transmit_sensor_lidar() {
  dbc_SENSOR_LIDAR_s sensor_lidar_data = {};
  lidar_data_handler__retrieve_distance();
  // within_range();
  lidar_data_handler__get_distances(&sensor_lidar_data);
  // printf("\nSensor values from SENSOR Node:\r\nLeft = %d\r\nRight = %d\r\nFront = %d\r\nRear = %d\r\n",
  //        sensor_lidar.SENSOR_LIDAR_slight_left, sensor_lidar.SENSOR_LIDAR_slight_right,
  //        sensor_lidar.SENSOR_LIDAR_middle, sensor_lidar.SENSOR_LIDAR_back);

  if (!dbc_encode_and_send_SENSOR_LIDAR(NULL, &sensor_lidar_data)) {
#if SENSOR_NODE_DEBUG == 1
    printf("Failed to encode and send Sensor Lidar\r\n");
#endif
  }
}

static void can_sensor__transmit_sensor_bt_coordinates() {
  dbc_SENSOR_BT_COORDINATES_s message_bt_coordinates = {};

  gps_coordinates_s dest_coordinates = {};
  bt_wrapper__get_destination_coordinates(&dest_coordinates);

  message_bt_coordinates.SENSOR_BT_COORDINATES_latitude = dest_coordinates.latitude;
  message_bt_coordinates.SENSOR_BT_COORDINATES_longitude = dest_coordinates.longitude;

  if (!dbc_encode_and_send_SENSOR_BT_COORDINATES(NULL, &message_bt_coordinates)) {
#if SENSOR_NODE_DEBUG == 1
    printf("Failed to encode and send Sensor BT Coordinates\r\n");
#endif
  }
}

static void can_sensor__transmit_motor_key() {
  dbc_MOTOR_KEY_s message = {};
  bt_wrapper__get_motor_key(&message);
  // printf("\nKey: %d", message.MOTOR_KEY_val);

  if (!dbc_encode_and_send_MOTOR_KEY(NULL, &message)) {
#if SENSOR_NODE_DEBUG == 1
    printf("Failed to encode and send SENSOR MOTOR Key\r\n");
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
    printf("\nSensor values from Ultrasonic sensor:\r\nLeft = %lf\r\nRight = %lf\r\nMiddle = %lf\r\n",
           (double)sensor_sonar.SENSOR_SONARS_left, (double)sensor_sonar.SENSOR_SONARS_right,
           (double)sensor_sonar.SENSOR_SONARS_middle);
#endif

    // TODO, Do other things here
    // ! Added sensor sonar processing code here
    gpio__reset(board_io__get_led2());
    can_sensor__update_driver_obstacle(&sensor_sonar);
  }
}

void can_sensor__decode_sensor_lidar(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_SENSOR_LIDAR(&sensor_lidar, header, bytes)) {
#if SENSOR_NODE_DEBUG == 1
    printf("\nSensor values from LIDAR sensor:\r\nLeft = %d\r\nRight = %d\r\nFront = %d\r\nRear = %d\r\n",
           sensor_lidar.SENSOR_LIDAR_slight_left, sensor_lidar.SENSOR_LIDAR_slight_right,
           sensor_lidar.SENSOR_LIDAR_middle, sensor_lidar.SENSOR_LIDAR_back);
#endif

    // TODO, Do other things here
    // ! Added sensor sonar processing code here
    // gpio_reset(board_io_get_led2());
    // can_sensor__update_driver_obstacle(&sensor_sonar);
  }
}

/**
 * STATIC FUNCTIONS
 */
static void can_sensor__update_driver_obstacle(dbc_SENSOR_SONARS_s *sonar) { driver_obstacle__process_input(sonar); }
