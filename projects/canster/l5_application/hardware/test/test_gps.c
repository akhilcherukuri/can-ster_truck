#include "unity.h"

#include <stdio.h>

// Mocks
#include "Mockclock.h"
#include "Mockuart.h"

#include "Mockqueue.h"

#include "Mockgpio.h"

// Use the real implementation (not mocks) for:
#include "line_buffer.h"

// Include the source we wish to test
// #include "gps.h"
#include "gps.c"

void setUp(void) {
  gpio_s unused;
  gpio__construct_with_function_ExpectAndReturn(UART3_PORT, UART3_TX, GPIO__FUNCTION_2, unused);
  gpio__construct_with_function_ExpectAndReturn(UART3_PORT, UART3_RX, GPIO__FUNCTION_2, unused);

  uint32_t clock;
  clock__get_peripheral_clock_hz_ExpectAndReturn(clock);
  uart__init_stop_bit_Expect(GPS_UART, clock, GPS_BAUD_RATE, 0);

  QueueHandle_t rxq_handle;
  QueueHandle_t txq_handle;
  xQueueCreate_ExpectAndReturn(RX_QUEUE_SIZE, sizeof(char), rxq_handle);
  xQueueCreate_ExpectAndReturn(TX_QUEUE_SIZE, sizeof(char), txq_handle);
  uart__enable_queues_ExpectAndReturn(GPS_UART, rxq_handle, txq_handle, true);

  line_buffer__init(&line, line_buffer, sizeof(line_buffer));

  gps__init();
}

void tearDown(void) { memset(line_buffer, 0, sizeof(line_buffer)); }

void test_gps_init(void) {
  // setUp runs here
  // dummy function for setup expects
}

void test_gps_absorb_data(void) {
  char byte;
  uart__get_ExpectAndReturn(GPS_UART, &byte, 0, true);
  uart__get_ExpectAndReturn(GPS_UART, &byte, 0, false);
  gps__absorb_data();

  TEST_ASSERT_EQUAL_INT(line.write_index, 1);
}

void test_gps_absorb_data_multiple(void) {
  char byte;
  uart__get_ExpectAndReturn(GPS_UART, &byte, 0, true);
  uart__get_ExpectAndReturn(GPS_UART, &byte, 0, true);
  uart__get_ExpectAndReturn(GPS_UART, &byte, 0, false);
  gps__absorb_data();

  TEST_ASSERT_EQUAL_INT(line.write_index, 2);
}

void test_gps_absorb_data_thruptr(void) {
  char data[] = "$GPRMC,043522.400,A,3718.0468,N,12155.5098,W,0.04,319.65,220320,,,A*76\r\n";

  char byte;
  for (int i = 0; i < strlen(data); i++) {
    uart__get_ExpectAndReturn(GPS_UART, NULL, 0, true);
    uart__get_IgnoreArg_input_byte();
    uart__get_ReturnThruPtr_input_byte(&data[i]);
  }
  uart__get_ExpectAndReturn(GPS_UART, NULL, 0, false);
  uart__get_IgnoreArg_input_byte();

  gps__absorb_data();

  char get_data[100];
  line_buffer__remove_line(&line, get_data, 100);
  TEST_ASSERT_EQUAL_STRING(get_data, "$GPRMC,043522.400,A,3718.0468,N,12155.5098,W,0.04,319.65,220320,,,A*76\r");
}

/**
 * TESTED GGA, GSV, GSA and RMC
 */
void test_gps_format_gps_line(void) {
  char gps_id[10] = {0};

  // gga
  char gps_input_gga[] = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
  gps__format_gps_line(gps_input_gga, gps_id);
  TEST_ASSERT_EQUAL_STRING(gps_id, "GPGGA");
  TEST_ASSERT_EQUAL_STRING(gps_input_gga, "$GPGGA 123519 4807.038 N 01131.000 E 1 08 0.9 545.4 M 46.9 M  *47\r\n");

  memset(gps_id, 0, sizeof(gps_id));

  // rmc
  char gps_input_rmc[] = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A";
  gps__format_gps_line(gps_input_rmc, gps_id);
  TEST_ASSERT_EQUAL_STRING(gps_id, "GPRMC");
  TEST_ASSERT_EQUAL_STRING(gps_input_rmc, "$GPRMC 123519 A 4807.038 N 01131.000 E 022.4 084.4 230394 003.1 W*6A");

  memset(gps_id, 0, sizeof(gps_id));

  // gsv
  char gps_input_gsv[] = "$GPGSV,2,1,08,01,40,083,46,02,17,308,41,12,07,344,39,14,22,228,45*75";

  gps__format_gps_line(gps_input_gsv, gps_id);
  TEST_ASSERT_EQUAL_STRING(gps_id, "GPGSV");
  TEST_ASSERT_EQUAL_STRING(gps_input_gsv, "$GPGSV 2 1 08 01 40 083 46 02 17 308 41 12 07 344 39 14 22 228 45*75");

  memset(gps_id, 0, sizeof(gps_id));

  // gsa
  char gps_input_gsa[] = "$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39";

  gps__format_gps_line(gps_input_gsa, gps_id);
  TEST_ASSERT_EQUAL_STRING(gps_id, "GPGSA");
  TEST_ASSERT_EQUAL_STRING(gps_input_gsa, "$GPGSA A 3 04 05  09 12   24     2.5 1.3 2.1*39");
}

// RMC
void test_gps_parse_rmc(void) {
  char gps_input[] = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n";
  for (int i = 0; i < sizeof(gps_input); i++) {
    TEST_ASSERT_TRUE(line_buffer__add_byte(&line, gps_input[i]));
    // ? debugging
    // printf("%x ", gps_line[i]);
  }

  gps__handle_line();

  TEST_ASSERT_EQUAL_STRING(parsed_rmc.id, "$GPRMC");
  TEST_ASSERT_EQUAL_UINT32(parsed_rmc.fix, 123519);
  TEST_ASSERT_EQUAL_CHAR(parsed_rmc.status, 'A');

  TEST_ASSERT_EQUAL_FLOAT(parsed_rmc.latitude, 4807.038);
  TEST_ASSERT_EQUAL_CHAR(parsed_rmc.latitude_direction, 'N');

  TEST_ASSERT_EQUAL_FLOAT(parsed_rmc.longitude, 1131.00);
  TEST_ASSERT_EQUAL_CHAR(parsed_rmc.longitude_direction, 'E');

  TEST_ASSERT_EQUAL_FLOAT(parsed_rmc.speed, 22.4);
  TEST_ASSERT_EQUAL_FLOAT(parsed_rmc.track_angle, 84.4);

  TEST_ASSERT_EQUAL_UINT32(parsed_rmc.date, 230394);

  // Get coordinates
  gps_coordinates_s gps_p = gps__get_coordinates();
  TEST_ASSERT_EQUAL_FLOAT(gps_p.latitude, 4807.038);
  TEST_ASSERT_EQUAL_FLOAT(gps_p.longitude, 1131.00);
}
