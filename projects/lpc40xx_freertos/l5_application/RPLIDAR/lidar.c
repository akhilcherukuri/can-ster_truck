#include "lidar.h"
#include <stdio.h>

static uart_e lidar_uart = UART__3;
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

static bool received_direct_response = false;
static int direct_response_counter = 0;

static int data_response_index = 0;
static char data_response[5] = {0};
static char line[100] = {0};

void lidar__config_init(void) {
  gpio__construct_with_function(GPIO__PORT_4, 28, GPIO__FUNCTION_2); // P4.28 as TXD3
  gpio__construct_with_function(GPIO__PORT_4, 29, GPIO__FUNCTION_2); // P4.29 as RXD3

  uart__init(lidar_uart, clock__get_peripheral_clock_hz(), 115200);

  QueueHandle_t rxq_handle = xQueueCreate(500, sizeof(char));
  QueueHandle_t txq_handle = xQueueCreate(50, sizeof(char));
  uart__enable_queues(lidar_uart, rxq_handle, txq_handle);

  gpio_s motoctl_pin = gpio__construct_as_output(GPIO__PORT_2, 0);
  gpio__set(motoctl_pin);
  // gpio__construct_with_function(GPIO__PORT_2, 0, GPIO__FUNCTION_2); // P0.1 as PWM
  // pwm1__init_single_edge(4000);
  // pwm1__set_duty_cycle(PWM1__2_0, 0.8);
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

  // delay__ms(100);
}

void lidar__sample_scan(void) {
  uint8_t request[] = {start_byte, sample_scan};

  // delay__ms(5000);
  for (int i = 0; i < 2; i++) {
    uart__put(lidar_uart, request[i], 0);
  }
}

void lidar__sample_scan_run_once(int send_once) {
  if (1 == send_once) {
    uint8_t request[] = {start_byte, sample_scan};

    // delay__ms(5000);
    received_direct_response = false;
    for (int i = 0; i < 2; i++) {
      uart__put(lidar_uart, request[i], 0);
    }
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

  // delay__ms(500);
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
  // delay__ms(1);
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

static bool found_start = false;
static int found_start_counter = 0;
static int counter = 0;
static int index = -1;
void lidar__receive_data_response(void) {
  char byte;
  // int i = 0;
  if (uart__get(lidar_uart, &byte, 0)) {
    if (byte == 0xa5) {
      found_start = true;
      found_start_counter = counter;
    } else if (found_start == true && byte == 0x5a) {
      index = counter;
      found_start = false;
    }
    if (found_start_counter + 1 == counter && byte != 0x5a) {
      found_start = false;
      found_start_counter = 0;
    }
    // line[i++] = byte;
    // printf("%d %x\r\n", index, byte);
    printf("%x\r\n", byte);
    counter++;
  }

  // printf("Line[0] = %x\r\n", line[0]);
  // printf("Line[1] = %x\r\n", line[1]);
  // printf("Line[2] = %x\r\n", line[2]);
  // printf("Line[3] = %x\r\n", line[3]);
  // printf("Line[4] = %x\r\n", line[4]);
  // printf("Line[5] = %x\r\n", line[5]);
  // printf("Line[6] = %x\r\n", line[6]);
  // if ((line[0] == 0xa5) && (line[1] == 0x5a) && (line[2] == 0x05) && (line[3] == 0x00) && (line[4] == 0x00) &&
  //     (line[5] == 0x40) && (line[6] == 0x81)) {
  //   can_led__led3_ON();
  // } else {
  //   can_led__led3_OFF();
  // }
}

void lidar__receive_data_response_check(void) {
  char byte;
  // int i = 0;
  if (uart__get(lidar_uart, &byte, 0)) {
    if (received_direct_response) {
      data_response[data_response_index++] = byte;
    } else if (!received_direct_response) {
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
      }
    }
  }
}