#include "bt_wrapper.h"

#include "bt.h"

#include <stdio.h>
#include <string.h>

/**
 * Static state variables
 */
static gps_coordinates_s destination_coordinate;

/**
 * Static function declaration
 */
static void bt_wrapper__read_cb(char *buffer, char *identifier);
static void bt_wrapper__update_decoded_messages(void);

// Static parsers
static void bt_wrapper__parse_loc(char *buffer);

/**
 * Functions
 */
void bt_wrapper__read_once(void) { bt__read(bt_wrapper__read_cb); }

void bt_wrapper__write_once(void) {
  bt_wrapper__update_decoded_messages();

  // TODO, Send the message here
}

void bt_wrapper__get_destination_coordinates(gps_coordinates_s *dest_coordinate) {
  *dest_coordinate = destination_coordinate;
}

/**
 * Static functions
 */
static void bt_wrapper__read_cb(char *buffer, char *identifier) {
  // Parse everything here
  if (strcmp(identifier, "$loc") == 0) {
    bt_wrapper__parse_loc(buffer);
  }
}

// TODO, Write this function once the DBC messages are finalized
static void bt_wrapper__update_decoded_messages(void) {}

/**
 * Static Parsers
 */
static void bt_wrapper__parse_loc(char *buffer) {
  char id[10] = {0};
  sscanf(buffer, "%s %f %f", id, &destination_coordinate.latitude, &destination_coordinate.longitude);

  printf("%f %f\r\n", (double)destination_coordinate.latitude, (double)destination_coordinate.longitude);
}
