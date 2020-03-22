#include "lidar.h"
#include <stdio.h>

static uart_e lidar_uart = UART__3;
static const uint8_t start_byte = 0xA5;
static const uint8_t sample_scan = 0x20;
static const uint8_t express_scan = 0x82;
uint8_t byte[150];

void lidar__config_init(void) {
  uart__init(lidar_uart, clock__get_peripheral_clock_hz(), 115200);

  QueueHandle_t rxq_handle = xQueueCreate(140, sizeof(char));
  QueueHandle_t txq_handle = xQueueCreate(8, sizeof(char)); // We don't send anything to the GPS
  uart__enable_queues(lidar_uart, rxq_handle, txq_handle);

  gpio__construct_with_function(GPIO__PORT_4, 28, GPIO__FUNCTION_2); // P0.1 as TXD3
  gpio__construct_with_function(GPIO__PORT_4, 29, GPIO__FUNCTION_2); // P0.1 as RXD3

  // gpio_s motoctl_pin = gpio__construct_as_output(GPIO__PORT_0, 17);
  // gpio__reset(motoctl_pin);
  pwm1__init_single_edge(2000);
  pwm1__set_duty_cycle(PWM1__2_0, 0.5);
}

void lidar__sample_scan(void) {
  uint8_t request[] = {start_byte, sample_scan};

  for (int i = 0; i < 100000; i++) {
  }
  for (int i = 0; i < 2; i++) {
    uart__put(lidar_uart, request[i], 0);
  }
}

void lidar__express_scan(void) {
  uint8_t request[] = {start_byte, express_scan, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22};

  for (int i = 0; i < 9; i++) {
    uart__put(lidar_uart, request[i], 0);
  }
}

void lidar__receive_data_response(void) {
  while (uart__get(lidar_uart, &byte, 0)) {
    printf("%x", byte);
  }
}
