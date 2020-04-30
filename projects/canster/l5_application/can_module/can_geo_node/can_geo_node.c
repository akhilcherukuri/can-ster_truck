#include "can_geo_node.h"

#include "compass.h"
#include "gps.h"

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
static dbc_GEO_DESTINATION_REACHED_s geo_destination_reached;

static dbc_GEO_CURRENT_COORDINATES_s geo_current_coordinates;
static dbc_GEO_DEBUG_s geo_debug;

/**
 * Functions
 */
// Return by copy (getters)
void can_geo__get_geo_degree(dbc_GEO_DEGREE_s *return_degree) { *return_degree = geo_degree; }
void can_geo__get_heartbeat(dbc_GEO_HEARTBEAT_s *return_heartbeat) { *return_heartbeat = geo_heartbeat; }
void can_geo__get_geo_destination_reached(dbc_GEO_DESTINATION_REACHED_s *return_destination_reached) {
  *return_destination_reached = geo_destination_reached;
}

// Debug
void can_geo__get_geo_current_coordinates(dbc_GEO_CURRENT_COORDINATES_s *return_current_coordinates) {
  *return_current_coordinates = geo_current_coordinates;
}
void can_geo__get_geo_debug(dbc_GEO_DEBUG_s *return_debug) { *return_debug = geo_debug; }

#if BOARD_GEO_NODE == 1

static void can_geo__transmit_geo_heartbeat();
static void can_geo__transmit_geo_degree();
static void can_geo__transmit_geo_destination_reached();

// Debug
static void can_geo__transmit_geo_current_coordinates();
static void can_geo__transmit_geo_debug();

// Transmit
void can_geo__transmit_all_messages(void) {
  can_geo__transmit_geo_heartbeat();
  can_geo__transmit_geo_degree();
  can_geo__transmit_geo_destination_reached();

  can_geo__transmit_geo_current_coordinates();
  can_geo__transmit_geo_debug();
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

static void can_geo__transmit_geo_destination_reached() {
  // TODO, Construct the message here from geo_logic
  dbc_GEO_DESTINATION_REACHED_s message = {};

  if (!dbc_encode_and_send_GEO_DESTINATION_REACHED(NULL, &message)) {
#if GEO_NODE_DEBUG == 1
    printf("Failed to encode and send Geo Destination reached\r\n");
#endif
  }
}

// Debug
static void can_geo__transmit_geo_current_coordinates() {
  gps_coordinates_s current_coordiantes = gps__get_coordinates();

  dbc_GEO_CURRENT_COORDINATES_s message = {};
  message.GEO_CURRENT_COORDINATES_latitude = current_coordiantes.latitude;
  message.GEO_CURRENT_COORDINATES_longitude = current_coordiantes.longitude;

  if (!dbc_encode_and_send_GEO_CURRENT_COORDINATES(NULL, &message)) {
#if GEO_NODE_DEBUG == 1
    printf("Failed to encode and send Geo Current Coordinates\r\n");
#endif
  }
}

static void can_geo__transmit_geo_debug() {
  // TODO, Construct message from geo logic
  dbc_GEO_DEBUG_s message = {};
  // message.GEO_DEBUG_current_waypoint_index
  // message.GEO_DEBUG_distance_from_destination

  if (!dbc_encode_and_send_GEO_DEBUG(NULL, &message)) {
#if GEO_NODE_DEBUG == 1
    printf("Failed to encode and send Geo Debug\r\n");
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

void can_geo__geo_destination_reached() {
  const uint32_t increment = 1000;

  if (dbc_service_mia_GEO_DESTINATION_REACHED(&geo_destination_reached, increment)) {
#if GEO_NODE_DEBUG == 1
    printf("MIA -> GEO_DESTINATION_REACHED\r\n");
#endif
  }

  // Do something here
}

/**
 * DECODE
 */
static void can_geo__on_decode_geo_degree(void);

void can_geo__decode_geo_heartbeat(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_GEO_HEARTBEAT(&geo_heartbeat, header, bytes)) {
#if GEO_NODE_DEBUG == 1
    printf("Geo Heartbeat: %d\r\n", geo_heartbeat.GEO_HEARTBEAT_cmd);
#endif

    // Do something here
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

void can_geo__decode_geo_destination_reached(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_GEO_DESTINATION_REACHED(&geo_destination_reached, header, bytes)) {
#if GEO_NODE_DEBUG == 1
    printf("Geo Destination Reached: %d\r\n", geo_destination_reached.GEO_DESTINATION_REACHED_cmd);
#endif

    // Do something here
  }
}

// Debug
void can_geo__decode_geo_current_coordinates(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_GEO_CURRENT_COORDINATES(&geo_current_coordinates, header, bytes)) {
#if GEO_NODE_DEBUG == 1
    printf("Geo Current Coordinates: Latitude %f, Longitude %f\r\n",
           (double)geo_current_coordinates.GEO_CURRENT_COORDINATES_latitude,
           (double)geo_current_coordinates.GEO_CURRENT_COORDINATES_longitude);
#endif

    // Do something here
  }
}

void can_geo__decode_geo_debug(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_GEO_DEBUG(&geo_debug, header, bytes)) {
#if GEO_NODE_DEBUG == 1
    printf("Geo Node Debug Waypoint: %d, Distance from Destination: %f\r\n", geo_debug.GEO_DEBUG_current_waypoint_index,
           (double)geo_debug.GEO_DEBUG_distance_from_destination);
#endif

    // Do something here
  }
}

static void can_geo__on_decode_geo_degree(void) { driver_obstacle__geo_controller_directions(&geo_degree); }
