#include "unity.h"

#include <string.h>

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockcan_led.h"

// Include the source we wish to test
#include "lidar_data_handler.c"

void setUp(void) {
  memset(left_data, 127, sizeof(left_data));
  memset(front_data, 127, sizeof(front_data));
  memset(right_data, 127, sizeof(right_data));
  memset(rear_data, 127, sizeof(rear_data));
}

void tearDown(void) {}

void test__min_distance(void) {
  uint16_t data[] = {90, 85, 60, 76, 105, 100, 49, 44, 66};

  TEST_ASSERT_EQUAL(44, min_distance(data, sizeof(data) / 2));
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
  TEST_ASSERT_NOT_EQUAL(distance_threshold + 20, rear_data[10]);
  lidar_data_response_handle_distance(180, distance_threshold + 20, 20);
  TEST_ASSERT_EQUAL(distance_threshold + 20, rear_data[10]);
  TEST_ASSERT_EQUAL(distance_threshold + 20, min_distance(rear_data, sizeof(rear_data) / 2));

  TEST_ASSERT_NOT_EQUAL(distance_threshold - 10, rear_data[20]);
  lidar_data_response_handle_distance(190, distance_threshold - 10, 20);
  TEST_ASSERT_EQUAL(distance_threshold - 10, rear_data[20]);
  TEST_ASSERT_EQUAL(distance_threshold - 10, min_distance(rear_data, sizeof(rear_data) / 2));

  TEST_ASSERT_NOT_EQUAL(distance_threshold - 1, rear_data[0]);
  lidar_data_response_handle_distance(170, distance_threshold - 1, 20);
  TEST_ASSERT_EQUAL(distance_threshold - 1, rear_data[0]);
  TEST_ASSERT_EQUAL(distance_threshold - 10, min_distance(rear_data, sizeof(rear_data) / 2));
}

void test__lidar_data_handler_objects_in_front(void) {
  TEST_ASSERT_NOT_EQUAL(distance_threshold + 20, front_data[9]);
  lidar_data_response_handle_distance(359, distance_threshold + 20, 20);
  TEST_ASSERT_EQUAL(distance_threshold + 20, front_data[9]);
  TEST_ASSERT_EQUAL(distance_threshold + 20, min_distance(front_data, sizeof(front_data) / 2));

  TEST_ASSERT_NOT_EQUAL(distance_threshold, front_data[10]);
  lidar_data_response_handle_distance(0, distance_threshold, 20);
  TEST_ASSERT_EQUAL(distance_threshold, front_data[10]);
  TEST_ASSERT_EQUAL(distance_threshold, min_distance(front_data, sizeof(front_data) / 2));

  TEST_ASSERT_NOT_EQUAL(2, front_data[9]);
  lidar_data_response_handle_distance(359, 2, 20);
  TEST_ASSERT_EQUAL(2, front_data[9]);
  TEST_ASSERT_EQUAL(2, min_distance(front_data, sizeof(front_data) / 2));

  TEST_ASSERT_NOT_EQUAL(20, front_data[20]);
  lidar_data_response_handle_distance(10, 20, 20);
  TEST_ASSERT_EQUAL(20, front_data[20]);
  TEST_ASSERT_EQUAL(2, min_distance(front_data, sizeof(front_data) / 2));

  TEST_ASSERT_NOT_EQUAL(distance_threshold + 100, front_data[20]);
  lidar_data_response_handle_distance(10, distance_threshold + 100, 20);
  TEST_ASSERT_EQUAL(distance_threshold + 100, front_data[20]);
  TEST_ASSERT_EQUAL(2, min_distance(front_data, sizeof(front_data) / 2));

  TEST_ASSERT_NOT_EQUAL(distance_threshold, front_data[9]);
  lidar_data_response_handle_distance(359, distance_threshold, 20);
  TEST_ASSERT_EQUAL(distance_threshold, front_data[9]);
  TEST_ASSERT_EQUAL(distance_threshold, min_distance(front_data, sizeof(front_data) / 2));
}

void test__lidar_data_handler_objects_in_left(void) {
  TEST_ASSERT_NOT_EQUAL(distance_threshold + 20, left_data[0]);
  lidar_data_response_handle_distance(330, distance_threshold + 20, 20);
  TEST_ASSERT_EQUAL(distance_threshold + 20, left_data[0]);
  TEST_ASSERT_EQUAL(distance_threshold + 20, min_distance(left_data, sizeof(left_data) / 2));

  TEST_ASSERT_NOT_EQUAL(distance_threshold, left_data[19]);
  lidar_data_response_handle_distance(349, distance_threshold, 20);
  TEST_ASSERT_EQUAL(distance_threshold, left_data[19]);
  TEST_ASSERT_EQUAL(distance_threshold, min_distance(left_data, sizeof(left_data) / 2));

  TEST_ASSERT_NOT_EQUAL(2, left_data[0]);
  lidar_data_response_handle_distance(330, 2, 20);
  TEST_ASSERT_EQUAL(2, left_data[0]);
  TEST_ASSERT_EQUAL(2, min_distance(left_data, sizeof(left_data) / 2));

  TEST_ASSERT_NOT_EQUAL(distance_threshold - 10, left_data[19]);
  lidar_data_response_handle_distance(349, distance_threshold - 10, 20);
  TEST_ASSERT_EQUAL(distance_threshold - 10, left_data[19]);
  TEST_ASSERT_EQUAL(2, min_distance(left_data, sizeof(left_data) / 2));

  TEST_ASSERT_NOT_EQUAL(distance_threshold + 100, left_data[19]);
  lidar_data_response_handle_distance(349, distance_threshold + 100, 20);
  TEST_ASSERT_EQUAL(distance_threshold + 100, left_data[19]);
  TEST_ASSERT_EQUAL(2, min_distance(left_data, sizeof(left_data) / 2));

  TEST_ASSERT_NOT_EQUAL(distance_threshold, left_data[0]);
  lidar_data_response_handle_distance(330, distance_threshold, 20);
  TEST_ASSERT_EQUAL(distance_threshold, left_data[0]);
  TEST_ASSERT_EQUAL(distance_threshold, min_distance(left_data, sizeof(left_data) / 2));
}

void test__lidar_data_handler_objects_in_right(void) {
  TEST_ASSERT_NOT_EQUAL(distance_threshold + 20, right_data[0]);
  lidar_data_response_handle_distance(11, distance_threshold + 20, 20);
  TEST_ASSERT_EQUAL(distance_threshold + 20, right_data[0]);
  TEST_ASSERT_EQUAL(distance_threshold + 20, min_distance(right_data, sizeof(right_data) / 2));

  TEST_ASSERT_NOT_EQUAL(distance_threshold, right_data[19]);
  lidar_data_response_handle_distance(30, distance_threshold, 20);
  TEST_ASSERT_EQUAL(distance_threshold, right_data[19]);
  TEST_ASSERT_EQUAL(distance_threshold, min_distance(right_data, sizeof(right_data) / 2));

  TEST_ASSERT_NOT_EQUAL(2, right_data[0]);
  lidar_data_response_handle_distance(11, 2, 20);
  TEST_ASSERT_EQUAL(2, right_data[0]);
  TEST_ASSERT_EQUAL(2, min_distance(right_data, sizeof(right_data) / 2));

  TEST_ASSERT_NOT_EQUAL(distance_threshold - 10, right_data[19]);
  lidar_data_response_handle_distance(30, distance_threshold - 10, 20);
  TEST_ASSERT_EQUAL(distance_threshold - 10, right_data[19]);
  TEST_ASSERT_EQUAL(2, min_distance(right_data, sizeof(right_data) / 2));

  TEST_ASSERT_NOT_EQUAL(distance_threshold + 100, right_data[19]);
  lidar_data_response_handle_distance(30, distance_threshold + 100, 20);
  TEST_ASSERT_EQUAL(distance_threshold + 100, right_data[19]);
  TEST_ASSERT_EQUAL(2, min_distance(right_data, sizeof(right_data) / 2));

  TEST_ASSERT_NOT_EQUAL(distance_threshold, right_data[0]);
  lidar_data_response_handle_distance(11, distance_threshold, 20);
  TEST_ASSERT_EQUAL(distance_threshold, right_data[0]);
  TEST_ASSERT_EQUAL(distance_threshold, min_distance(right_data, sizeof(right_data) / 2));
}

void test__check_bits_start_correct(void) {
  uint8_t data[] = {0x1, 0x1, 0x0, 0x0, 0x0};
  TEST_ASSERT_TRUE(check_bits(data));
}

void test__check_bits_NOT_start_correct(void) {
  uint8_t data[] = {0x2, 0x1, 0x0, 0x0, 0x0};
  TEST_ASSERT_TRUE(check_bits(data));
}

void test__check_bits_incorrect_start(void) {
  uint8_t data1[] = {0b01101111, 0x1, 0x0, 0x0, 0x0};
  TEST_ASSERT_FALSE(check_bits(data1));

  uint8_t data2[] = {0b01101100, 0x1, 0x0, 0x0, 0x0};
  TEST_ASSERT_FALSE(check_bits(data2));
}

void test__check_bits_start_NOT_correct(void) {
  uint8_t data[] = {0x1, 0x0, 0x0, 0x0, 0x0};
  TEST_ASSERT_FALSE(check_bits(data));
}

void test__check_bits_NOT_start_NOT_correct(void) {
  uint8_t data[] = {0x2, 0x0, 0x0, 0x0, 0x0};
  TEST_ASSERT_FALSE(check_bits(data));
}

void test__data_response_parse_left(void) {
  uint16_t angle = 330;
  uint16_t dist = 50;
  uint8_t data[5];
  data[0] = 0x3e;

  for (angle = left_low_angle - 1; angle < front_low_angle; angle++) {
    data[1] = ((angle << 7) | 0x1) & 0xff;
    data[2] = (angle >> 1) & 0xff;

    if (angle != left_low_angle - 1) {
      dist = distance_threshold - 1;
      data[3] = (dist << 2) & 0xff;
      data[4] = (dist >> 6) & 0xff;
      TEST_ASSERT_NOT_EQUAL(dist, left_data[(angle - left_low_angle) % 20]);
      lidar_data_handler__response_parse(data);
      TEST_ASSERT_EQUAL(dist, left_data[(angle - left_low_angle) % 20]);

      dist = distance_threshold;
      data[3] = (dist << 2) & 0xff;
      data[4] = (dist >> 6) & 0xff;
      TEST_ASSERT_NOT_EQUAL(dist, left_data[(angle - left_low_angle) % 20]);
      lidar_data_handler__response_parse(data);
      TEST_ASSERT_EQUAL(dist, left_data[(angle - left_low_angle) % 20]);
    } else {
      lidar_data_handler__response_parse(data);
    }
  }
}

void test__data_response_parse_right(void) {
  uint16_t angle = 330;
  uint16_t dist = 50;
  uint8_t data[5];
  data[0] = 0x3e;

  for (angle = right_low_angle; angle < right_high_angle + 1; angle++) {
    data[1] = ((angle << 7) | 0x1) & 0xff;
    data[2] = (angle >> 1) & 0xff;

    if (angle != right_high_angle + 1) {
      dist = distance_threshold - 1;
      data[3] = (dist << 2) & 0xff;
      data[4] = (dist >> 6) & 0xff;
      TEST_ASSERT_NOT_EQUAL(dist, right_data[(angle - right_low_angle) % 20]);
      lidar_data_handler__response_parse(data);
      TEST_ASSERT_EQUAL(dist, right_data[(angle - right_low_angle) % 20]);

      dist = distance_threshold;
      data[3] = (dist << 2) & 0xff;
      data[4] = (dist >> 6) & 0xff;
      TEST_ASSERT_NOT_EQUAL(dist, right_data[(angle - right_low_angle) % 20]);
      lidar_data_handler__response_parse(data);
      TEST_ASSERT_EQUAL(dist, right_data[(angle - right_low_angle) % 20]);
    } else {
      lidar_data_handler__response_parse(data);
    }
  }
}

void test__data_response_parse_rear(void) {
  uint16_t angle = 330;
  uint16_t dist = 50;
  uint8_t data[5];
  data[0] = 0x3e;

  for (angle = rear_low_angle - 1; angle < rear_high_angle + 1; angle++) {
    data[1] = ((angle << 7) | 0x1) & 0xff;
    data[2] = (angle >> 1) & 0xff;

    if ((angle != rear_low_angle - 1) && (angle != rear_high_angle + 1)) {
      dist = distance_threshold - 1;
      data[3] = (dist << 2) & 0xff;
      data[4] = (dist >> 6) & 0xff;
      TEST_ASSERT_NOT_EQUAL(dist, rear_data[(angle - rear_low_angle) % 21]);
      lidar_data_handler__response_parse(data);
      TEST_ASSERT_EQUAL(dist, rear_data[(angle - rear_low_angle) % 21]);

      dist = distance_threshold;
      data[3] = (dist << 2) & 0xff;
      data[4] = (dist >> 6) & 0xff;
      TEST_ASSERT_NOT_EQUAL(dist, rear_data[(angle - rear_low_angle) % 21]);
      lidar_data_handler__response_parse(data);
      TEST_ASSERT_EQUAL(dist, rear_data[(angle - rear_low_angle) % 21]);
    } else {
      lidar_data_handler__response_parse(data);
    }
  }
}

void test__data_response_parse_front(void) {
  uint16_t angle = 330;
  uint16_t dist = 50;
  uint8_t data[5];
  data[0] = 0x3e;

  for (uint16_t mod_angle = front_low_angle; mod_angle < circle_end_angle + front_high_angle + 1; mod_angle++) {
    angle = mod_angle % 360;

    data[1] = ((angle << 7) | 0x1) & 0xff;
    data[2] = (angle >> 1) & 0xff;
    dist = distance_threshold - 1;
    data[3] = (dist << 2) & 0xff;
    data[4] = (dist >> 6) & 0xff;
    TEST_ASSERT_NOT_EQUAL(dist, front_data[(mod_angle - front_low_angle) % 21]);
    lidar_data_handler__response_parse(data);
    TEST_ASSERT_EQUAL(dist, front_data[(mod_angle - front_low_angle) % 21]);

    dist = distance_threshold;
    data[3] = (dist << 2) & 0xff;
    data[4] = (dist >> 6) & 0xff;
    TEST_ASSERT_NOT_EQUAL(dist, front_data[(mod_angle - front_low_angle) % 21]);
    lidar_data_handler__response_parse(data);
    TEST_ASSERT_EQUAL(dist, front_data[(mod_angle - front_low_angle) % 21]);
  }
}
