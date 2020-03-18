#include "can_driver_node.h"
#include "board_io.h"
#include "gpio.h"
#include "project.h"

#define DRIVER_NODE_DEBUG 1

#if DRIVER_NODE_DEBUG == 1
#include <stdio.h>
#endif

int threshold = 100;

static dbc_DRIVER_HEARTBEAT_s driver_heartbeat;
static dbc_SENSOR_SONARS_s decoded_sensor;
static dbc_MOTOR_SPEED_s motor_speed;
static dbc_MOTOR_STEERING_s motor_steering;
static dbc_MOTOR_SPEED_FEEDBACK_s motor_speed_feedback;

const dbc_SENSOR_SONARS_s *can_sensor__get_sensor_sonar() { return &decoded_sensor; }
const dbc_DRIVER_HEARTBEAT_s *can_driver__get_heartbeat() { return &driver_heartbeat; }
const dbc_MOTOR_SPEED_FEEDBACK_s *can_motor__get_motor_speed_feedback() { return &motor_speed_feedback; }

void can_driver__driver_heartbeat_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_SENSOR_HEARTBEAT(&driver_heartbeat, mia_increment_value)) {
#if SENSOR_NODE_DEBUG == 1
    printf("MIA -> DRIVER_HEARTBEAT\r\n");
    printf("assigned default sensor heartbeat = %d\r\n", driver_heartbeat.DRIVER_HEARTBEAT_cmd);
#endif

    // Do other things here
    gpio__set(board_io__get_led2());
  }
}
void can_driver__sensor_sonar_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_SENSOR_SONARS(&decoded_sensor, mia_increment_value)) {
#if DRIVER_NODE_DEBUG == 1
    printf("MIA -> SENSOR_SONARS\r\n");
    printf("assigned default sensor sonar values = %d:%d:%d\r\n", decoded_sensor.SENSOR_SONARS_left,
           decoded_sensor.SENSOR_SONARS_middle, decoded_sensor.SENSOR_SONARS_right);

#endif

    // Do other things here
    gpio__set(board_io__get_led3());
  }
}
void can_driver__motor_speed_feedback_mia() {
  const uint32_t mia_increment_value = 1000;

  if (dbc_service_mia_MOTOR_SPEED_FEEDBACK(&motor_speed_feedback, mia_increment_value)) {
#if DRIVER_NODE_DEBUG == 1
    printf("MIA -> DRIVER_MOTOR_SPEED_FEEDBACK\r\n");
    printf("assigned default motor speed feedback value= %f\r\n", motor_speed_feedback.MOTOR_SPEED_current);
#endif
    // Do other things here
    gpio__set(board_io__get_led1());
  }
}
void can_driver__handle_incoming_message(dbc_message_header_t *header, uint8_t data[8]) {
  if (dbc_decode_SENSOR_SONARS(&decoded_sensor, *header, data)) {
#if DRIVER_NODE_DEBUG == 1
    printf("\nR_Left Sensor:%d,R_Right Sensor:%d,R_Middle Sensor:%d\r\n", decoded_sensor.SENSOR_SONARS_left,
           decoded_sensor.SENSOR_SONARS_right, decoded_sensor.SENSOR_SONARS_middle);
#endif
  }
  if (dbc_decode_MOTOR_SPEED_FEEDBACK(&motor_speed_feedback, *header, data)) {
#if DRIVER_NODE_DEBUG == 1
    printf("\nMOTOR Feedback speed: %f\r\n", motor_speed_feedback.MOTOR_SPEED_current);
#endif
  }
  if (dbc_decode_DRIVER_HEARTBEAT(&driver_heartbeat, *header, data)) {
#if DRIVER_NODE_DEBUG == 1
    printf("\nDRIVER Heartbeat: %d\r\n", driver_heartbeat.DRIVER_HEARTBEAT_cmd);
#endif
  }
}

#if BOARD_DRIVER_NODE == 1

static void can_driver__transmit_driver_heartbeat();
static void can_driver__transmit_motor_steering_speed();
void can_sensor__transmit_all_messages(void) {

  can_driver__transmit_driver_heartbeat();
  can_driver__transmit_motor_steering_speed();
}

static void can_sensor__transmit_sensor_heartbeat() {
  dbc_DRIVER_HEARTBEAT_s message = {{0}, DRIVER_HEARTBEAT_cmd_SYNC};
  if (!dbc_encode_and_send_DRIVER_HEARTBEAT(NULL, &message)) {
#if DRIVER_NODE_DEBUG == 1
    printf("Failed to encode and send Driver Heartbeat\r\n");
#endif
  }
}

static void can_driver__transmit_motor_steering_speed() {
  if (decoded_sensor.SENSOR_SONARS_middle <= threshold) {

    if (decoded_sensor.SENSOR_SONARS_left <= threshold && decoded_sensor.SENSOR_SONARS_right > threshold) {
      motor_speed.MOTOR_SPEED_processed = (motor_speed_feedback.MOTOR_SPEED_current * 0.5);
      motor_steering.MOTOR_STEERING_direction = -2; // steer right
      motor_speed.MOTOR_SPEED_processed = (motor_speed_feedback.MOTOR_SPEED_current / 0.6);
    } else if (decoded_sensor.SENSOR_SONARS_right <= threshold && decoded_sensor.SENSOR_SONARS_left > threshold) {
      motor_speed.MOTOR_SPEED_processed = (motor_speed_feedback.MOTOR_SPEED_current * 0.5);
      motor_steering.MOTOR_STEERING_direction = 2; // steer left
      motor_speed.MOTOR_SPEED_processed = (motor_speed_feedback.MOTOR_SPEED_current / 0.6);
    } else if (decoded_sensor.SENSOR_SONARS_right <= threshold && decoded_sensor.SENSOR_SONARS_left <= threshold) {
      motor_speed.MOTOR_SPEED_processed = (motor_speed_feedback.MOTOR_SPEED_current * 0.0);
    } else {
      motor_speed.MOTOR_SPEED_processed = (motor_speed_feedback.MOTOR_SPEED_current * 0.5);
      motor_steering.MOTOR_STEERING_direction = 2;
      motor_speed.MOTOR_SPEED_processed = (motor_speed_feedback.MOTOR_SPEED_current / 0.6);
    }
  } else {
    motor_speed.MOTOR_SPEED_processed = 60;
    motor_steering.MOTOR_STEERING_direction = 0; // steer straight
  }
  if (!dbc_encode_and_send_MOTOR_STEERING(NULL, &motor_steering)) {
#if DRIVER_NODE_DEBUG == 1
    printf("Failed to encode and send Motor Steering");
#endif
  }

  if (dbc_encode_and_send_MOTOR_SPEED(NULL, &motor_speed);) {
#if DRIVER_NODE_DEBUG == 1
    printf("Failed to encode and send Motor Speed");
#endif
  }
}
#else
void can_driver__transmit_all_messages(void) {}
#endif
