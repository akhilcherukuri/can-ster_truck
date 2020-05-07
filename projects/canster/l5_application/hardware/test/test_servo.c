#include "unity.h"

#include "Mockgpio.h"
#include "Mockpwm1.h"

#include "servo.c"

void setUp() {}
void tearDown() {}

void test_servo__configure_pwm_channel(void) {
  gpio_s unused;
  gpio__construct_with_function_ExpectAndReturn(PWM_GPIO_PORT, PWM_GPIO_PIN, PWM_GPIO_FUNCTION, unused);
  servo__configure_pwm_channel();
}

static void test_servo_set_duty_helper(float duty_cycle) { pwm1__set_duty_cycle_Expect(PWM_CHANNEL, duty_cycle); }

void test_servo__steer_set_duty_cycle(void) {
  test_servo_set_duty_helper(10);
  servo__steer_set_duty_cycle(10);

  test_servo_set_duty_helper(21.12);
  servo__steer_set_duty_cycle(21.12);
}

void test_servo__steer_hard_left(void) {
  test_servo_set_duty_helper(HARD_LEFT);
  servo__steer_hard_left();
}

void test_servo__steer_hard_right(void) {
  test_servo_set_duty_helper(HARD_RIGHT);
  servo__steer_hard_right();
}

void test_servo__steer_soft_left(void) {
  test_servo_set_duty_helper(SOFT_LEFT);
  servo__steer_soft_left();
}

void test_servo__steer_soft_right(void) {
  test_servo_set_duty_helper(SOFT_RIGHT);
  servo__steer_soft_right();
}

void test_servo__steer_straight(void) {
  test_servo_set_duty_helper(STRAIGHT);
  servo__steer_straight();
}

/**
 * Function Test
 */
static void servo__steer_set_duty_cycle(float duty_cycle);

/**
 * NON-STATIC FUNCTION DEFINITIONS
 */
void test_servo__steer_init(void) {
  pwm1__init_single_edge_Expect(PWM_FREQUENCY);

  gpio_s unused;
  gpio__construct_with_function_ExpectAndReturn(PWM_GPIO_PORT, PWM_GPIO_PIN, PWM_GPIO_FUNCTION, unused);

  servo__steer_init();
}

void test_servo__steer_processor(void) {

  test_servo_set_duty_helper(HARD_LEFT);
  test_servo_set_duty_helper(SOFT_LEFT);
  test_servo_set_duty_helper(STRAIGHT);
  test_servo_set_duty_helper(SOFT_RIGHT);
  test_servo_set_duty_helper(HARD_RIGHT);

  for (int i = MOTOR_STEERING_hard_left; i <= MOTOR_STEERING_hard_right; i++) {
    servo__steer_processor(i);
  }
}
