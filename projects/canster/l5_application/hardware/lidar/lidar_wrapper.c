#include "lidar_wrapper.h"

void lidar__init(void) { lidar__config_init(); }

void lidar__run_1hz(int callback_count) { lidar__scan_run_once(callback_count); }

void lidar__run_10hz() {}

void lidar__run_100Hz(int callback_count) {
  //   if (callback_count % 5 == 0) {
  //     lidar_data_handler__retrieve_distance();
  //     // within_range();
  //   }
}

void lidar__run_1000Hz(void) { lidar__receive_data_response_check(); }