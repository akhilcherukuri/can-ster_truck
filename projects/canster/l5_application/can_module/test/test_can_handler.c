#include "unity.h"

#include "Mockcan_bus.h"
#include "Mockcan_driver_node.h"
#include "Mockcan_motor_node.h"
#include "Mockcan_sensor_node.h"

// Not Mocked
#include "can_bus_constants.h"
#include "can_handler.h"
#include "can_mia_configurations.c"

/**
 * Write your tests here
 */
void setUp(void) {}
void tearDown(void) {}

void test_dbc_send_can_message(void) {
  can__msg_t message = {.data.bytes = {0x11, 0x22, 0x33, 0x44, 0, 0, 0, 0}};
  message.msg_id = 10;
  message.frame_fields.data_len = 4;
  can__tx_ExpectAndReturn(CAN_PORT, &message, 0, true);

  uint8_t message_id = 10;
  uint8_t bytes[8] = {0x11, 0x22, 0x33, 0x44, 0, 0, 0, 0};
  uint8_t dlc = 4;
  dbc_send_can_message(NULL, message_id, bytes, dlc);
}