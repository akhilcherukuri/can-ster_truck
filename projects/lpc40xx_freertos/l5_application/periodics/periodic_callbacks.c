#include "periodic_callbacks.h"
#include "can_led.h"
#include "lidar.h"

#include "board_io.h"
#include "gpio.h"

/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */
void periodic_callbacks__initialize(void) {
  // This method is invoked once when the periodic tasks are created
  can_led__init();
  lidar__config_init();
}

void periodic_callbacks__1Hz(uint32_t callback_count) {
  // Add your code here
  // lidar__sample_scan();
  lidar__express_scan();
}

void periodic_callbacks__10Hz(uint32_t callback_count) {
  // Add your code here
}
void periodic_callbacks__100Hz(uint32_t callback_count) {
  // Add your code here
  lidar__receive_data_response();
}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {
  // Add your code here
}