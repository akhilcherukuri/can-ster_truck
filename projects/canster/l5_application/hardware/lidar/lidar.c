#include "lidar.h"
#include <stdio.h>

static uart_e lidar_uart = UART__3;

/**
 * STATIC CONST VARIABLES
 */
static const uint8_t start_byte = 0xa5;

static const uint8_t lidar_stop = 0x25;
static const uint8_t lidar_reset = 0x40;
static const uint8_t sample_scan = 0x20;
static const uint8_t express_scan = 0x82;
static const uint8_t force_scan = 0x21;

static const uint8_t lidar_info = 0x50;
static const uint8_t lidar_health = 0x52;
static const uint8_t lidar_samplerate = 0x59;
static const uint8_t lidar_conf = 0x84;

/**
 * STATIC VARIABLES
 */
static bool received_direct_response = false;
static int direct_response_counter = 0;
static int data_response_index = 0;
static uint8_t data_response[5] = {0};

/**
 * STATIC FUNCTIONS
 */
static bool direct_response_check(char byte);

void lidar__config_init(void) {
  gpio__construct_with_function(GPIO__PORT_4, 28, GPIO__FUNCTION_2); // P4.28 as TXD3
  gpio__construct_with_function(GPIO__PORT_4, 29, GPIO__FUNCTION_2); // P4.29 as RXD3

  uart__init(lidar_uart, clock__get_peripheral_clock_hz(), 115200);

  QueueHandle_t rxq_handle = xQueueCreate(2000, sizeof(char));
  QueueHandle_t txq_handle = xQueueCreate(50, sizeof(char));

  uart__enable_queues(lidar_uart, rxq_handle, txq_handle);

  lidar_data_handler_init();
}

void lidar__stop(void) {
  uint8_t request[] = {start_byte, lidar_stop};

  for (int i = 0; i < 2; i++) {
    uart__put(lidar_uart, request[i], 0);
  }
}

void lidar__reset(void) {
  uint8_t request[] = {start_byte, lidar_reset};

  for (int i = 0; i < 2; i++) {
    uart__put(lidar_uart, request[i], 0);
  }
}

void lidar__sample_scan(void) {
  uint8_t request[] = {start_byte, sample_scan};

  for (int i = 0; i < 2; i++) {
    uart__put(lidar_uart, request[i], 0);
  }
}

void lidar__sample_scan_run_once(int send_once) {
  if (1 == send_once) {
    uint8_t request[] = {start_byte, sample_scan};

    received_direct_response = false;
    for (int i = 0; i < 2; i++) {
      uart__put(lidar_uart, request[i], 0);
    }
    gpio_s motoctl_pin = gpio__construct_as_output(GPIO__PORT_2, 0);
    gpio__set(motoctl_pin);
  }
}

void lidar__express_scan(void) {
  uint8_t request[] = {start_byte, express_scan, 0x05, 0x03, 0x00, 0x00, 0x00, 0x00, 0x21};

  for (int i = 0; i < 9; i++) {
    uart__put(lidar_uart, request[i], 0);
  }
}

void lidar__force_scan(void) {
  uint8_t request[] = {start_byte, force_scan};

  for (int i = 0; i < 2; i++) {
    uart__put(lidar_uart, request[i], 0);
  }
}

void lidar__get_info(void) {
  uint8_t request[] = {start_byte, lidar_info};

  for (int i = 0; i < 2; i++) {
    uart__put(lidar_uart, request[i], 0);
  }
}

void lidar__get_health(void) {
  uint8_t request[] = {start_byte, lidar_health};

  for (int i = 0; i < 2; i++) {
    uart__put(lidar_uart, request[i], 0);
  }
}

void lidar__get_samplerate(void) {
  uint8_t request[] = {start_byte, lidar_samplerate};

  for (int i = 0; i < 2; i++) {
    uart__put(lidar_uart, request[i], 0);
  }
}

void lidar__get_conf(void) {
  uint8_t request[] = {start_byte, lidar_conf, 0x01, 0x70, 0x50};

  for (int i = 0; i < 5; i++) {
    uart__put(lidar_uart, request[i], 0);
  }
}

static bool direct_response_check(char byte) {
  received_direct_response = false;
  if (byte == 0xa5 && direct_response_counter == 0) {
    direct_response_counter++;
  } else if (byte == 0x5a && direct_response_counter == 1) {
    direct_response_counter++;
  } else if (byte == 0x5 && direct_response_counter == 2) {
    direct_response_counter++;
  } else if (byte == 0x0 && (direct_response_counter == 3 || direct_response_counter == 4)) {
    direct_response_counter++;
  } else if (byte == 0x40 && direct_response_counter == 5) {
    direct_response_counter++;
  } else if (byte == 0x81 && direct_response_counter == 6) {
    received_direct_response = true;
  } else {
    direct_response_counter = 0;
  }
  return received_direct_response;
}

void lidar__receive_data_response_check(void) {
  char byte;
  while (uart__get(lidar_uart, &byte, 0)) {
    if (received_direct_response) {
      // can_led__led0_ON();
      // can_led__led1_OFF();
      // data_response[data_response_index++] = byte;

      // printf("%x\r\n", byte);
      if (receive_five_byte_sample(byte)) {
        lidar_data_response_parse_v2();
      }
    } else if (!received_direct_response) {
      // can_led__led1_ON();
      // can_led__led0_OFF();
      // printf("%x\r\n", byte);
      received_direct_response = direct_response_check(byte);
    }

    // if (data_response_index > 4) {
    //   printf("Computing Data\r\n");
    //   data_response_index = 0;
    //   lidar_data_response_parse(data_response);
    // }
    // data_response_index = (data_response_index + 1) % 5;
  }
}