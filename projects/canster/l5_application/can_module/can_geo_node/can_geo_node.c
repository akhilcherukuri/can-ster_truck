#include "can_geo_node.h"

#include "board_io.h"
#include "gpio.h"

#include "compass.h"

#include "driver_obstacle.h"
#include "geo_logic.h"

/**
 * Constants
 */
#define GEO_NODE_DEBUG 1

#if GEO_NODE_DEBUG == 1
#include <stdio.h>
#endif

/**
 * STATIC VARIABLES
 */
static dbc_GEO_HEARTBEAT_s geo_heartbeat;
static dbc_GEO_DEGREE_s geo_degree;
static dbc_GEO_DESTINATION_REACHED_s geo_destination;

/**
 * GETTERS
 */
const dbc_GEO_DEGREE_s *can_geo__get_geo_degree() { return &geo_degree; }
const dbc_GEO_HEARTBEAT_s *can_geo__get_heartbeat() { return &geo_heartbeat; }

#if BOARD_GEO_NODE == 1

/**
 * STATIC FUNCTION DECLARATIONS
 */
static void can_geo__transmit_geo_heartbeat();
static void can_geo__transmit_geo_degree();

/**
 * TRANSMIT
 */
void can_geo__transmit_all_messages(void) {
  can_geo__transmit_geo_heartbeat();
  can_geo__transmit_geo_degree();
}

/**
 * STATIC FUNCTIONS
 */
static void can_geo__transmit_geo_heartbeat() {
  dbc_GEO_HEARTBEAT_s message = {};
  message.GEO_HEARTBEAT_cmd = GEO_HEARTBEAT_cmd_SYNC;
  if (!dbc_encode_and_send_GEO_HEARTBEAT(NULL, &message)) {
#if GEO_NODE_DEBUG == 1
    printf("\nFailed to encode and send Geo Heartbeat");
#endif
  }
}

static void can_geo__transmit_geo_degree() {
  // TODO, get current and computed degree from here
  dbc_GEO_DEGREE_s message = {};
  message.GEO_DEGREE_current = compass__read_compass_bearing_16bit();
  message.GEO_DEGREE_required = geo_logic__compute_required_bearing();

  if (!dbc_encode_and_send_GEO_DEGREE(NULL, &message)) {
#if GEO_NODE_DEBUG == 1
    printf("\nFailed to encode and send Geo Degree");
#endif
  }
}

#else
void can_geo__transmit_all_messages() {}
#endif

/**
 * MIA
 */
void can_geo__geo_heartbeat_mia() {
  const uint32_t increment = 1000;
  if (dbc_service_mia_GEO_HEARTBEAT(&geo_heartbeat, increment)) {
#if GEO_NODE_DEBUG == 1
    printf("\nMIA -> GEO_HEARTBEAT");
    printf("\nAssigned default geo heartbeat: %d", geo_heartbeat.GEO_HEARTBEAT_cmd);
#endif
    gpio__set(board_io__get_led1());
  }
}

void can_geo__geo_degree_mia() {
  const uint32_t increment = 1000;

  if (dbc_service_mia_GEO_DEGREE(&geo_degree, increment)) {
#if GEO_NODE_DEBUG == 1
    printf("\nMIA -> GEO DEGREE");
    printf("\nAssigned default geo degree: %f %f", (double)geo_degree.GEO_DEGREE_current,
           (double)geo_degree.GEO_DEGREE_required);
#endif
  }
}

void can_geo__geo_destination_reached_mia() {
  const uint32_t increment = 1000;
  if (dbc_service_mia_GEO_DESTINATION_REACHED(&geo_destination, increment)) {
#if GEO_NODE_DEBUG == 1
    printf("\nMIA -> GEO_DESTINATION_REACHED");
#endif
  }
}

/**
 * DECODE
 */
static void can_geo__on_decode_geo_degree(void);

void can_geo__decode_geo_heartbeat(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_GEO_HEARTBEAT(&geo_heartbeat, header, bytes)) {
#if GEO_NODE_DEBUG == 1
    printf("\nGeo Heartbeat: %d\r\n", geo_heartbeat.GEO_HEARTBEAT_cmd);
#endif

    gpio__reset(board_io__get_led1());
  }
}

void can_geo__decode_geo_degree(dbc_message_header_t header, uint8_t bytes[8]) {

  if (dbc_decode_GEO_DEGREE(&geo_degree, header, bytes)) {
#if GEO_NODE_DEBUG == 1
    printf("\nGeo Degree: current(from compass): %lf, required(from computed haversine bearing angle): %lf",
           (double)(geo_degree.GEO_DEGREE_current), (double)geo_degree.GEO_DEGREE_required);
#endif
    can_geo__on_decode_geo_degree();
  }
}

void can_geo__decode_geo_destination_reached(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_GEO_DESTINATION_REACHED(&geo_destination, header, bytes)) {
#if GEO_NODE_DEBUG == 1
    printf("\nGeo Destination Reached: %d", geo_destination.GEO_DESTINATION_REACHED_cmd);
#endif
    if (geo_destination.GEO_DESTINATION_REACHED_cmd == GEO_DESTINATION_REACHED_cmd_REACHED) {
      set_destination_reached_status(true);
    } else {
      set_destination_reached_status(false);
    }
  }
}

/* Only for testing */
static void can_geo__on_decode_geo_degree(void) {
  driver_obstacle__set_geo_controller_direction(&geo_degree);
  int16_t steer_dir = driver_obstacle__move_to_destination();
  printf("\nComputed steering value based on GEO curr/reqd logic %d", steer_dir);
}
