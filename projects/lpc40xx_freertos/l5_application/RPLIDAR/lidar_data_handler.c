#include "lidar_data_handler.h"

// static char data_response[5] = {0};

static uint8_t measurement_quality = 0;
static uint16_t measurement_angle = 0;
static uint16_t measurement_distance = 0;

static const uint16_t left_low_angle = 330;
static const uint16_t left_high_angle = 349;
static const uint16_t front_low_angle = 350;
static const uint16_t front_high_angle = 10;
static const uint16_t right_low_angle = 11;
static const uint16_t right_high_angle = 30;
static const uint16_t rear_low_angle = 170;
static const uint16_t rear_high_angle = 190;

static const uint16_t circle_start_angle = 0;
static const uint16_t circle_end_angle = 359;

static uint16_t distance_left = 500;
static uint16_t distance_right = 500;
static uint16_t distance_front = 500;
static uint16_t distance_rear = 500;
static uint8_t distance_threshold = 30;

static uint16_t left_data[20];
static uint16_t front_data[21];
static uint16_t right_data[20];
static uint16_t rear_data[21];

void lidar_data_handler_init(void) {
  memset(&left_data, 127, sizeof(left_data));
  memset(&front_data, 127, sizeof(front_data));
  memset(&right_data, 127, sizeof(right_data));
  memset(&rear_data, 127, sizeof(rear_data));
}

static uint16_t min_distance(uint16_t *distance_data, uint8_t data_size) {
  uint16_t min_dist = distance_data[0];

  for (int i = 1; i < data_size; i++) {
    if (min_dist > distance_data[i])
      min_dist = distance_data[i];
  }

  return min_dist;
}

static void lidar_data_response_handle_distance(uint16_t angle, uint16_t distance, uint8_t quality) {
  printf("%u   %u\r\n", angle, distance);
  if (quality > 0xe && distance != 0) {
    if (angle >= left_low_angle && angle <= left_high_angle) {
      left_data[(angle - left_low_angle) % 20] = distance;
      distance_left = min_distance(&left_data, sizeof(left_data) / 2);
      //   printf("%u   %u\r\n", angle, distance_left);
      if (distance_left < distance_threshold) {
        can_led__led0_ON();
      } else {
        can_led__led0_OFF();
      }
    } else if (angle >= right_low_angle && angle <= right_high_angle) {
      right_data[(angle - right_low_angle) % 20] = distance;
      distance_right = min_distance(&right_data, sizeof(right_data) / 2);
      //   printf("%u   %u\r\n", angle, distance_right);
      if (distance_right < distance_threshold) {
        can_led__led1_ON();
      } else {
        can_led__led1_OFF();
      }
    } else if (angle >= rear_low_angle && angle <= rear_high_angle) {
      rear_data[(angle - rear_low_angle) % 21] = distance;
      distance_rear = min_distance(&rear_data, sizeof(rear_data) / 2);
      //   printf("%u   %u\r\n", angle, distance_rear);
      if (distance_rear < distance_threshold) {
        can_led__led2_ON();
      } else {
        can_led__led2_OFF();
      }
    } else if (angle >= front_low_angle && angle <= circle_end_angle) {
      front_data[(angle - front_low_angle) % 10] = distance;
      distance_front = min_distance(&front_data, sizeof(front_data) / 2);
      //   printf("%u   %u\r\n", angle, distance_front);
      if (distance_front < distance_threshold) {
        can_led__led3_ON();
      } else {
        can_led__led3_OFF();
      }
    } else if (angle >= circle_start_angle && angle <= front_high_angle) {
      front_data[((angle - circle_start_angle) % 11) + 10] = distance;
      distance_front = min_distance(&front_data, sizeof(front_data) / 2);
      //   printf("%u   %u\r\n", angle, distance_front);
      if (distance_front < distance_threshold) {
        can_led__led3_ON();
      } else {
        can_led__led3_OFF();
      } // remove
    }
  }
}

static bool check_bits(uint8_t *byte) {
  //   bool check_bit = false;

  //   if (((byte[0] & 0x1) == ((~(byte[0] >> 1) & 0x1)) && (byte[1] & 0x1))) {
  //     check_bit = true;
  //   }

  //   return check_bit;
  return ((byte[0] & 0x1) == ((~(byte[0] >> 1) & 0x1)) && (byte[1] & 0x1));
}

void lidar_data_response_parse(uint8_t *data) {
  if (check_bits(data)) {
    measurement_quality = (data[0] >> 2);
    measurement_angle = (((data[2] << 7) | (data[1] >> 1)) >> 6);
    measurement_distance = (((data[4] << 8) | (data[3])) >> 2);

    // for (int i = 0; i < 5; i++) {
    //   printf("%x\r\n", data[i]);
    // }
    lidar_data_response_handle_distance(measurement_angle, measurement_distance, measurement_quality);
  }
}

void check_range(void) {
  distance_left = min_distance(&left_data, sizeof(left_data) / 2);
  distance_right = min_distance(&right_data, sizeof(right_data) / 2);
  distance_rear = min_distance(&rear_data, sizeof(rear_data) / 2);
  distance_front = min_distance(&front_data, sizeof(front_data) / 2);
}

void within_range(void) {
  if (distance_left < distance_threshold) {
    can_led__led0_ON();
  } else {
    can_led__led0_OFF();
  }
  if (distance_right < distance_threshold) {
    can_led__led1_ON();
  } else {
    can_led__led1_OFF();
  }
  if (distance_rear < distance_threshold) {
    can_led__led2_ON();
  } else {
    can_led__led2_OFF();
  }
  if (distance_front < distance_threshold) {
    can_led__led3_ON();
  } else {
    can_led__led3_OFF();
  }
  if (distance_front < distance_threshold) {
    can_led__led3_ON();
  } else {
    can_led__led3_OFF();
  }
}
