#include "can_led.h"

static gpio_s switch_3;
gpio_s led3;

void can_led__init(void) {
  switch_3 = gpio__construct_as_input(GPIO__PORT_0, 29);
  led3 = board_io__get_led3();
  gpio__set(board_io__get_led0());
  gpio__set(board_io__get_led1());
  gpio__set(board_io__get_led2());
  gpio__set(led3);
}

bool can_led__switch_pressed(void) { return gpio__get(switch_3); }

void can_led__led3_ON(void) { gpio__reset(led3); }

void can_led__led3_OFF(void) { gpio__set(led3); }
