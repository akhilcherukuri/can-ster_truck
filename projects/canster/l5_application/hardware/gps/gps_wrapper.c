#include "gps_wrapper.h"

#include "gpio.h"
#include "gps.h"

gpio_s valid_gpio;

void gps_wrapper__init() {
  valid_gpio = gpio__construct_as_output(GPIO__PORT_0, 6);
  gpio__reset(valid_gpio);
}

void gps_wrapper__update_led_when_valid() {
  if (gps__is_valid()) {
    gpio__set(valid_gpio);
  } else {
    gpio__reset(valid_gpio);
  }
}
