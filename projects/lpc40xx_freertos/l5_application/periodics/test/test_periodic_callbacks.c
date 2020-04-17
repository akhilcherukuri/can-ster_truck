#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockboard_io.h"
#include "Mockcan_led.h"
#include "Mockgpio.h"
#include "Mocklidar.h"
#include "Mocklidar_data_handler.h"

// Include the source we wish to test
#include "periodic_callbacks.h"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  can_led__init_Expect();
  lidar__config_init_Expect();
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {
  lidar__scan_run_once_Expect(0);
  periodic_callbacks__1Hz(0);

  lidar__scan_run_once_Expect(1);
  periodic_callbacks__1Hz(1);
}

void test__periodic_callbacks__100Hz(void) {
  lidar_data_handler__retrieve_distance_Expect();
  within_range_Expect();
  periodic_callbacks__100Hz(0);
}

void test__periodic_callbacks__1000Hz(void) {
  lidar__receive_data_response_check_Expect();
  periodic_callbacks__1000Hz(0);
}
