#include "unity.h"

#include "Mockdelay.h"
#include "Mockgpio.h"
#include "Mockpwm1.h"

#include "esc.c"

void test_esc__init() {
  gpio_s unused_gpio;
  gpio__construct_with_function_ExpectAndReturn(PWM_GPIO_PORT, PWM_GPIO_PIN, PWM_GPIO_FUNCTION, unused_gpio);
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, NEUTRAL);
  delay__ms_Expect(300);
  esc__init();
}

void test_esc__forward_slow() {
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, FORWARD_SLOW);
  esc__forward_slow();
}

void test_esc__forward_medium() {
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, FORWARD_MEDIUM);
  esc__forward_medium();
}

void test_esc__forward_fast() {
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, FORWARD_FAST);
  esc__forward_fast();
}
void test_esc__neutral() {
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, NEUTRAL);
  esc__neutral();
}

void test_esc__reverse_slow() {
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, REVERSE_SLOW);
  esc__reverse_slow();
}

void test_esc__reverse_medium() {
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, REVERSE_MEDIUM);
  esc__reverse_medium();
}

void test_esc__reverse_fast() {
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, REVERSE_FAST);
  esc__reverse_fast();
}

void test_esc__configure_pwm_channel(void) {
  gpio_s unused_gpio;
  gpio__construct_with_function_ExpectAndReturn(PWM_GPIO_PORT, PWM_GPIO_PIN, PWM_GPIO_FUNCTION, unused_gpio);
  esc__configure_pwm_channel();
}

void test_esc__set_duty_cycle() {
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, REVERSE_FAST);
  esc__set_duty_cycle(REVERSE_FAST);
}

void test_esc__direction_processor() {
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, REVERSE_FAST);
  esc__direction_processor(0);
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, REVERSE_MEDIUM);
  esc__direction_processor(1);
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, REVERSE_SLOW);
  esc__direction_processor(2);
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, NEUTRAL);
  esc__direction_processor(3);
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, FORWARD_SLOW);
  esc__direction_processor(4);
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, FORWARD_MEDIUM);
  esc__direction_processor(5);
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, FORWARD_FAST);
  esc__direction_processor(6);
}