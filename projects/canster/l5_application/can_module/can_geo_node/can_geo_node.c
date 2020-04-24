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
 * Static function declaration
 */

/**
 * Static variables
 */
static dbc_GEO_HEARTBEAT_s geo_heartbeat;
static dbc_GEO_DEGREE_s geo_degree;

/**
 * Functions
 */
// Getters for all static variables
// ! NO SETTERS
// ! DO NOT DISCARD THE CONST QUALIFIER
const dbc_GEO_DEGREE_s *can_geo__get_geo_degree() { return &geo_degree; }
const dbc_GEO_HEARTBEAT_s *can_geo__get_heartbeat() { return &geo_heartbeat; }

#if BOARD_GEO_NODE == 1

static void can_geo__transmit_geo_heartbeat();
static void can_geo__transmit_geo_degree();

// Transmit
void can_geo__transmit_all_messages(void) {
  can_geo__transmit_geo_heartbeat();
  can_geo__transmit_geo_degree();
}

static void can_geo__transmit_geo_heartbeat() {
  dbc_GEO_HEARTBEAT_s message = {};
  message.GEO_HEARTBEAT_cmd = GEO_HEARTBEAT_cmd_SYNC;
  if (!dbc_encode_and_send_GEO_HEARTBEAT(NULL, &message)) {
#if GEO_NODE_DEBUG == 1
    printf("Failed to encode and send Geo Heartbeat\r\n");
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
    printf("Failed to encode and send Geo Degree\r\n");
#endif
  }
}

#else
void can_geo__transmit_all_messages() {}
#endif

// Mia
void can_geo__geo_heartbeat_mia() {
  const uint32_t increment = 1000;
  if (dbc_service_mia_GEO_HEARTBEAT(&geo_heartbeat, increment)) {
#if GEO_NODE_DEBUG == 1
    printf("MIA -> GEO_HEARTBEAT\r\n");
    printf("Assigned default geo heartbeat: %d\r\n", geo_heartbeat.GEO_HEARTBEAT_cmd);
#endif

    // Do something here
    gpio__set(board_io__get_led1());
  }
}

void can_geo__geo_degree_mia() {
  const uint32_t increment = 1000;

  if (dbc_service_mia_GEO_DEGREE(&geo_degree, increment)) {
#if GEO_NODE_DEBUG == 1
    printf("MIA -> GEO DEGREE\r\n");
    printf("Assigned default geo degree: %f %f\r\n", (double)geo_degree.GEO_DEGREE_current,
           (double)geo_degree.GEO_DEGREE_required);
#endif

    // Do something here
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

    // Do something here
    gpio__reset(board_io__get_led1());
  }
}
void can_geo__decode_geo_degree(dbc_message_header_t header, uint8_t bytes[8]) {

  if (dbc_decode_GEO_DEGREE(&geo_degree, header, bytes)) {
#if GEO_NODE_DEBUG == 1
    printf("Geo Degree: current: %f, required: %f\r\n", (double)geo_degree.GEO_DEGREE_current,
           (double)geo_degree.GEO_DEGREE_required);
#endif

    // Do something here
    can_geo__on_decode_geo_degree();
  }
}

static void can_geo__on_decode_geo_degree(void) { driver_obstacle__geo_controller_directions(&geo_degree); }
