#include "can_geo_node.h"

#include "compass.h"

#include "driver_obstacle.h"
#include "geo_logic.h"

#include "gps.h"

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
static dbc_GEO_DESTINATION_REACHED_s geo_destination;

/**
 * Functions
 */
// Getters for all static variables
dbc_GEO_DEGREE_s can_geo__get_geo_degree() { return geo_degree; }
dbc_GEO_HEARTBEAT_s can_geo__get_heartbeat() { return geo_heartbeat; }

#if BOARD_GEO_NODE == 1

static void can_geo__transmit_geo_heartbeat();
static void can_geo__transmit_geo_degree();
static void can_geo__transmit_geo_destination_reached();
static void can_geo__transmit_geo_distance_from_destination();

// Debug Message
static void can_geo__transmit_geo_coordinates_debug();

// Transmit
void can_geo__transmit_all_messages(void) {
  can_geo__transmit_geo_heartbeat();
  can_geo__transmit_geo_degree();
  can_geo__transmit_geo_destination_reached();
  can_geo__transmit_geo_distance_from_destination();

  can_geo__transmit_geo_coordinates_debug();
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
  dbc_GEO_DEGREE_s message = {};

  // Actual values
  message.GEO_DEGREE_current = compass__read_compass_bearing_16bit();
  message.GEO_DEGREE_required = geo_logic__compute_required_bearing();

  if (!dbc_encode_and_send_GEO_DEGREE(NULL, &message)) {
#if GEO_NODE_DEBUG == 1
    printf("Failed to encode and send Geo Degree\r\n");
#endif
  } else {
    printf("\nCurrent compass degree = %lf Required compass degree = %lf\r\n", (double)message.GEO_DEGREE_current,
           (double)message.GEO_DEGREE_required);
  }
}

static void can_geo__transmit_geo_destination_reached() {
  dbc_GEO_DESTINATION_REACHED_s message = {};
  message = geo_logic__compute_destination_reached();

  if (!dbc_encode_and_send_GEO_DESTINATION_REACHED(NULL, &message)) {
#if GEO_NODE_DEBUG == 1
    printf("\nFailed to encode and send Geo Destination");
#endif
  } else {
    printf("\nDestination reached val: (1 - yes, 0 - no): %d", message.GEO_DESTINATION_REACHED_cmd);
  }
}

// Debug
static void can_geo__transmit_geo_coordinates_debug() {

  gps_coordinates_s coordinates = gps__get_coordinates();
  dbc_GEO_CURRENT_COORDINATES_s message = {};

  message.GEO_CURRENT_COORDINATES_latitude = coordinates.latitude;
  message.GEO_CURRENT_COORDINATES_longitude = coordinates.longitude;

  if (!dbc_encode_and_send_GEO_CURRENT_COORDINATES(NULL, &message)) {
#if GEO_NODE_DEBUG == 1
    printf("Failed to encode and send Geo Coordinates\r\n");
#endif
  }
}

static void can_geo__transmit_geo_distance_from_destination() {
  dbc_GEO_DISTANCE_FROM_DESTINATION_s message;
  message.GEO_distance_from_destination = geo_logic__distance_from_destination();

  if (!dbc_encode_and_send_GEO_DISTANCE_FROM_DESTINATION(NULL, &message)) {
#if GEO_NODE_DEBUG == 1
    printf("Failed to encode and send Geo Distance from Destination\r\n");
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

    // ! Do something here
  }
}

void can_geo__geo_destination_reached_mia() {
  const uint32_t increment = 1000;
  if (dbc_service_mia_GEO_DESTINATION_REACHED(&geo_destination, increment)) {
#if GEO_NODE_DEBUG == 1
    printf("MIA -> GEO_DESTINATION_REACHED\r\n");
#endif

    // ! Do something here
  }
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
    printf("Compass Degree from GEO Node: Current: %f, Required: %f\r\n", (double)geo_degree.GEO_DEGREE_current,
           (double)geo_degree.GEO_DEGREE_required);
#endif

    // Do something here
    // can_geo__on_decode_geo_degree();
  }
}

void can_geo__decode_geo_destination_reached(dbc_message_header_t header, uint8_t bytes[8]) {
  if (dbc_decode_GEO_DESTINATION_REACHED(&geo_destination, header, bytes)) {
#if GEO_NODE_DEBUG == 1
    printf("Geo Destination Reached: %d\r\n", geo_destination.GEO_DESTINATION_REACHED_cmd);
#endif

    // Do something here
  }
}

void can_geo__decode_geo_current_coordinates_debug(dbc_message_header_t header, uint8_t bytes[8]) {
  dbc_GEO_CURRENT_COORDINATES_s gps_current_coordinate = {};
  if (dbc_decode_GEO_CURRENT_COORDINATES(&gps_current_coordinate, header, bytes)) {
#if GEO_NODE_DEBUG == 1
    printf("Current Latitude: %f, Longitude: %f\r\n", (double)gps_current_coordinate.GEO_CURRENT_COORDINATES_latitude,
           (double)gps_current_coordinate.GEO_CURRENT_COORDINATES_longitude);
#endif

    // TODO, Put a function here
    // TODO, Debug to Bluetooth or Driver!
  }
}

static void can_geo__on_decode_geo_degree(void) {
  driver_obstacle__geo_controller_directions(&geo_degree);
  printf("on_decode_geo_degree: %f %f\r\n", (double)geo_degree.GEO_DEGREE_current,
         (double)geo_degree.GEO_DEGREE_required);
}
