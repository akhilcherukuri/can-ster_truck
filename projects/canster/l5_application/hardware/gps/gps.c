#include "gps.h"

#include <stdio.h>

#include "clock.h"
#include "uart.h"

#include "gpio.h"

#include "line_buffer.h"

/**
 * Definitions and Constants
 */
// UART
static const gpio__port_e UART3_PORT = GPIO__PORT_4;
static const uint8_t UART3_TX = 28;
static const uint8_t UART3_RX = 29;
static const uint32_t GPS_BAUD_RATE = 9600;
static const uart_e GPS_UART = UART__3;

// DATA QUEUE
static const uint32_t RX_QUEUE_SIZE = 200;
static const uint32_t TX_QUEUE_SIZE = 4;

/**
 * Typedefs
 */
/**
 * GPS RMC:
 *     RMC          Recommended Minimum sentence C
 *     123519       Fix taken at 12:35:19 UTC
 *     A            Status A=active or V=Void.
 *     4807.038,N   Latitude 48 deg 07.038' N
 *     01131.000,E  Longitude 11 deg 31.000' E
 *     022.4        Speed over the ground in knots
 *     084.4        Track angle in degrees True
 *     230394       Date - 23rd of March 1994
 *
 *    ! DISCARDED
 *     003.1,W      Magnetic Variation
 *     *6A          The checksum data, always begins with
 */
typedef struct {
  char id[10];
  float fix;
  char status;

  float latitude;
  char latitude_direction;

  float longitude;
  char longitude_direction;

  float speed;
  float track_angle;
  uint32_t date;
} gps_rmc_s;

/**
 * STATIC FUNCTION DECLARATIONS
 */
static void gps__absorb_data();

static void gps__handle_line();
static void gps__format_gps_line(char *gps_line, char *gps_id);
static void gps__parse_rmc(char *gps_line);

static void gps__update_coordinates(float latitude, float longitude);

/**
 * Static variables
 */
static gps_rmc_s parsed_rmc;
static gps_coordinates_s coordinates;

// Space for the line buffer, and the line buffer data structure instance
static char line_buffer[512];
static line_buffer_s line;

/**
 * Functions
 */
void gps__init() {
  gpio__construct_with_function(UART3_PORT, UART3_TX, GPIO__FUNCTION_2);
  gpio__construct_with_function(UART3_PORT, UART3_RX, GPIO__FUNCTION_2);

  // (1 << 2) is for stop bit is 2
  uart__init_stop_bit(GPS_UART, clock__get_peripheral_clock_hz(), GPS_BAUD_RATE, 0);

  QueueHandle_t rxq_handle = xQueueCreate(RX_QUEUE_SIZE, sizeof(char));
  QueueHandle_t txq_handle = xQueueCreate(TX_QUEUE_SIZE, sizeof(char));
  uart__enable_queues(GPS_UART, rxq_handle, txq_handle);

  line_buffer__init(&line, line_buffer, sizeof(line_buffer));
}

void gps__run_once(void) {
  gps__absorb_data();
  gps__handle_line();
}

gps_coordinates_s gps__get_coordinates(void) { return coordinates; }

bool gps__is_valid(void) {
  bool rval = false;
  if (parsed_rmc.status == 'A') {
    rval = true;
  }

  return rval;
}

/**
 * STATIC FUNCTION
 */
static void gps__absorb_data() {
  char byte;
  while (uart__get(GPS_UART, &byte, 0)) {
    line_buffer__add_byte(&line, byte);
  }
}

static void gps__handle_line() {
  // Id related data
  char gps_line[200] = {0};
  char gps_id[10] = {0};

  if (line_buffer__remove_line(&line, gps_line, sizeof(gps_line))) {

    // ? debugging
    printf("Data: %s\r\n", gps_line);
    gps__format_gps_line(gps_line, gps_id);

    // For now just parse gga
    // DONE, Parse other NMEA strings using gps_id
    if (strcmp(gps_id, "GPRMC") == 0) {
      gps__parse_rmc(gps_line);
    }
  }
}

static void gps__format_gps_line(char *gps_line, char *gps_id) {

  bool found_id = false;
  int id_counter = 0;

  // DONE: Parse the line to store GPS coordinates etc.
  // NOTE: This one for loop does 2 things
  // 1. Check for $ sign and parse out the initial ID (this happens before replacing , with SPACE)
  // 2. Check for , and replace with SPACE (to parse with sscanf)
  // ! IMP: ORDER MATTERS, Please do not change the order of the if statements
  // TODO: Make these if statments into their own functions (less error prone to improve the code)
  for (int i = 0; i < strlen(gps_line); i++) {

    if (found_id) {
      if (gps_line[i] == ',') {
        found_id = false;
      } else {
        gps_id[id_counter] = gps_line[i];
        id_counter++;
      }
    }

    if (gps_line[i] == '$') {
      found_id = true;
    }

    // TODO, replace with * as well
    if (gps_line[i] == ',') {
      gps_line[i] = ' ';
    }
  }
}

static void gps__parse_rmc(char *gps_line) {
  char discard[50];
  sscanf(gps_line, "%s %f %c %f %c %f %c %f %f %lu %s", parsed_rmc.id, &parsed_rmc.fix, &parsed_rmc.status,
         &parsed_rmc.latitude, &parsed_rmc.latitude_direction, &parsed_rmc.longitude, &parsed_rmc.longitude_direction,
         &parsed_rmc.speed, &parsed_rmc.track_angle, &parsed_rmc.date, discard);
  gps__update_coordinates(parsed_rmc.latitude, parsed_rmc.longitude);
}

static void gps__update_coordinates(float latitude, float longitude) {
  coordinates.latitude = latitude;
  coordinates.longitude = longitude;
}

/**
 * GPS DEBUG
 */
#if GPS_DEBUG == 1

void gps__debug_print_raw_data() {
  char byte;
  while (uart__get(GPS_UART, &byte, 0)) {
    printf("%c -> %x\r\n", byte, byte);
  }
  printf("-----\r\n");
}

void gps__debug_print_parsed_rmc_data() {
  printf("Fix Time: %f, Status: %c\r\n", (double)parsed_rmc.fix, parsed_rmc.status);
  printf("Speed: %f, TrackAngle: %f, Date: %ld\r\n", (double)parsed_rmc.speed, (double)parsed_rmc.track_angle,
         parsed_rmc.date);
}

#endif
