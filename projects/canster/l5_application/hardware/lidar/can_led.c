#include "can_led.h"

static gpio_s switch_3;
static gpio_s led0, led1, led2, led3;

void can_led__init(void) {
  switch_3 = gpio__construct_as_input(GPIO__PORT_0, 29);
  led0 = board_io__get_led0();
  led1 = board_io__get_led1();
  led2 = board_io__get_led2();
  led3 = board_io__get_led3();
  gpio__set(led0);
  gpio__set(led1);
  gpio__set(led2);
  gpio__set(led3);
}

bool can_led__switch3_pressed(void) { return gpio__get(switch_3); }

void can_led__led0_ON(void) { gpio__reset(led0); }
void can_led__led0_OFF(void) { gpio__set(led0); }
void can_led__led1_ON(void) { gpio__reset(led1); }
void can_led__led1_OFF(void) { gpio__set(led1); }
void can_led__led2_ON(void) { gpio__reset(led2); }
void can_led__led2_OFF(void) { gpio__set(led2); }
void can_led__led3_ON(void) { gpio__reset(led3); }
void can_led__led3_OFF(void) { gpio__set(led3); }
