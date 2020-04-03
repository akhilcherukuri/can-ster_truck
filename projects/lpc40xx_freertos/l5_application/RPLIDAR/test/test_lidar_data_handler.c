#include "unity.h"

#include <string.h>

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockcan_led.h"

// Include the source we wish to test
#include "lidar_data_handler.c"

void setUp(void) {
  //   static uint16_t left_data[19] = {500};
  memset(&left_data, 500, sizeof(left_data));
  memset(&rear_data, 500, sizeof(rear_data));
  memset(&right_data, 500, sizeof(right_data));
  memset(&front_data, 500, sizeof(front_data));
  //   static uint16_t front_data[21] = {500};
  //   static uint16_t right_data[19] = {500};
  //   static uint16_t rear_data[21] = {500};
}

void tearDown(void) {}

void test__min_distance(void) {
  uint16_t data[] = {90, 85, 60, 76, 105, 100, 49, 44, 66};

  TEST_ASSERT_EQUAL(44, min_distance(&data, sizeof(data) / 2));
}

void test__lidar_data_handler_receive_low_quality_data(void) {
  lidar_data_response_handle_distance(90, 50, 10);
  lidar_data_response_handle_distance(31, 10, 10);
  lidar_data_response_handle_distance(328, 20, 10);
  lidar_data_response_handle_distance(191, 20, 10);
}

void test__lidar_data_handler_objects_in_blind_spots_and_do_nothing(void) {
  lidar_data_response_handle_distance(90, 50, 20);
  lidar_data_response_handle_distance(31, 10, 20);
  lidar_data_response_handle_distance(328, 20, 20);
  lidar_data_response_handle_distance(191, 20, 20);
}

void test__lidar_data_handler_objects_in_rear(void) {
  can_led__led2_OFF_Expect();
  lidar_data_response_handle_distance(180, 50, 20);

  can_led__led2_ON_Expect();
  lidar_data_response_handle_distance(190, 20, 20);

  can_led__led2_ON_Expect();
  lidar_data_response_handle_distance(170, 29, 20);
}

void test__lidar_data_handler_objects_in_front_left_and_right(void) {
  //   can_led__led3_OFF_Expect();
  //   lidar_data_response_handle_distance(0, 30, 20);

  //   can_led__led3_ON_Expect();
  //   lidar_data_response_handle_distance(10, 20, 20);

  can_led__led1_ON_Expect();
  lidar_data_response_handle_distance(11, 29, 20);

  //   can_led__led1_OFF_Expect();
  //   lidar_data_response_handle_distance(28, 100, 20);

  can_led__led1_ON_Expect();
  lidar_data_response_handle_distance(28, 2, 20);

  can_led__led3_ON_Expect();
  lidar_data_response_handle_distance(359, 2, 20);

  can_led__led3_OFF_Expect();
  lidar_data_response_handle_distance(359, 50, 20);
}
