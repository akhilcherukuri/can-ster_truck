#include "unity.h"

#include "Mockdelay.h"
#include "Mockgpio.h"
#include "Mockpwm1.h"

#include "Mockrpm.h"
#include "esc.c"

void setUp() { offset = 0; }
void tearDown() { offset = 0; }

void get_speed_calculate_helper() {
#if FEEDBACK_LOOP == 1
  // get_speed_kph_ExpectAndReturn(0);
  get_speed_kph_ExpectAndReturn(0);
#endif
}

void test_esc__init() {
  gpio_s unused_gpio;
  gpio__construct_with_function_ExpectAndReturn(PWM_GPIO_PORT, PWM_GPIO_PIN, PWM_GPIO_FUNCTION, unused_gpio);
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, NEUTRAL);
  delay__ms_Expect(300);
  get_speed_calculate_helper();
  esc__init();
}

void test_esc__forward_slow() {
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, FORWARD_SLOW);
  get_speed_calculate_helper();
  esc__forward_slow();
}

void test_esc__forward_medium() {
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, FORWARD_MEDIUM);
  get_speed_calculate_helper();
  esc__forward_medium();
}

void test_esc__forward_fast() {
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, FORWARD_FAST);
  get_speed_calculate_helper();
  esc__forward_fast();
}
void test_esc__neutral() {
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, NEUTRAL);
  get_speed_calculate_helper();
  esc__neutral();
}

void test_esc__reverse_slow() {
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, REVERSE_SLOW);
  get_speed_calculate_helper();
  esc__reverse_slow();
}

void test_esc__reverse_medium() {
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, REVERSE_MEDIUM);
  get_speed_calculate_helper();
  esc__reverse_medium();
}

void test_esc__reverse_fast() {
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, REVERSE_FAST);
  get_speed_calculate_helper();
  esc__reverse_fast();
}

void test_esc__configure_pwm_channel(void) {
  gpio_s unused_gpio;
  gpio__construct_with_function_ExpectAndReturn(PWM_GPIO_PORT, PWM_GPIO_PIN, PWM_GPIO_FUNCTION, unused_gpio);
  esc__configure_pwm_channel();
}

void test_esc__set_duty_cycle() {
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, REVERSE_FAST);
  get_speed_calculate_helper();
  esc__set_duty_cycle(REVERSE_FAST);
}

void test_esc__direction_processor() {
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, REVERSE_FAST);
  pwm1__set_duty_cycle_IgnoreArg_duty_cycle_in_percent();
  get_speed_calculate_helper();
  esc__direction_processor(MOTOR_SPEED_reverse_fast);
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, REVERSE_MEDIUM);
  pwm1__set_duty_cycle_IgnoreArg_duty_cycle_in_percent();
  get_speed_calculate_helper();
  esc__direction_processor(MOTOR_SPEED_reverse_medium);
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, REVERSE_SLOW);
  pwm1__set_duty_cycle_IgnoreArg_duty_cycle_in_percent();
  get_speed_calculate_helper();
  esc__direction_processor(MOTOR_SPEED_reverse_slow);
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, NEUTRAL);
  pwm1__set_duty_cycle_IgnoreArg_duty_cycle_in_percent();
  get_speed_calculate_helper();
  esc__direction_processor(MOTOR_SPEED_neutral);
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, FORWARD_SLOW);
  pwm1__set_duty_cycle_IgnoreArg_duty_cycle_in_percent();
  get_speed_calculate_helper();
  esc__direction_processor(MOTOR_SPEED_forward_slow);
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, FORWARD_MEDIUM);
  pwm1__set_duty_cycle_IgnoreArg_duty_cycle_in_percent();
  get_speed_calculate_helper();
  esc__direction_processor(MOTOR_SPEED_forward_medium);
  pwm1__set_duty_cycle_Expect(PWM_CHANNEL, FORWARD_FAST);
  pwm1__set_duty_cycle_IgnoreArg_duty_cycle_in_percent();
  get_speed_calculate_helper();
  esc__direction_processor(MOTOR_SPEED_forward_fast);
}

void test_esc__set_duty_cycle_with_rpm_feedback_equal() {
#if FEEDBACK_LOOP == 1
  desired_direction_value = MOTOR_SPEED_forward_medium;
  float duty_cycle = FORWARD_MEDIUM;
  desired_speed_kph = FORWARD_MEDIUM_SPEED_KPH;            // Speed to go at
  get_speed_kph_ExpectAndReturn(FORWARD_MEDIUM_SPEED_KPH); // Current speed of car
  // printf("Hit: %f %f", duty_cycle, desired_speed_kph);
  // pwm1__set_duty_cycle_Expect(PWM_CHANNEL, 16.0);
  pwm1__set_duty_cycle_ExpectAnyArgs();

  esc__set_duty_cycle(duty_cycle);
  TEST_ASSERT_EQUAL_FLOAT(offset, 0.0);

#endif
}

void test_esc__set_duty_cycle_with_rpm_feedback_higher() {
#if FEEDBACK_LOOP == 1
  desired_direction_value = MOTOR_SPEED_forward_medium;
  float duty_cycle = FORWARD_MEDIUM;
  desired_speed_kph = FORWARD_MEDIUM_SPEED_KPH;          // Speed to go at
  get_speed_kph_ExpectAndReturn(FORWARD_FAST_SPEED_KPH); // Current speed of car
  pwm1__set_duty_cycle_ExpectAnyArgs();
  esc__set_duty_cycle(duty_cycle);
  TEST_ASSERT_EQUAL_FLOAT(offset, -PWM_ADJUSTMENT_OFFSET);
#endif
}

void test_esc__set_duty_cycle_with_rpm_feedback_lower() {
#if FEEDBACK_LOOP == 1
  desired_direction_value = MOTOR_SPEED_forward_medium;
  float duty_cycle = FORWARD_MEDIUM;
  desired_speed_kph = FORWARD_MEDIUM_SPEED_KPH;
  get_speed_kph_ExpectAndReturn(FORWARD_SLOW_SPEED_KPH);
  pwm1__set_duty_cycle_ExpectAnyArgs();
  esc__set_duty_cycle(duty_cycle);
  TEST_ASSERT_EQUAL_FLOAT(offset, PWM_ADJUSTMENT_OFFSET);
#endif
}

void test_speed_to_pwm_adjustment_if_car_is_faster_than_expected_forward() {
  desired_direction_value = MOTOR_SPEED_forward_medium;
  speed_to_pwm_adjustment(FORWARD_SLOW_SPEED_KPH, FORWARD_FAST_SPEED_KPH);
  TEST_ASSERT_EQUAL_FLOAT(offset, -PWM_ADJUSTMENT_OFFSET);
  desired_direction_value = MOTOR_SPEED_forward_medium;
  speed_to_pwm_adjustment(FORWARD_SLOW_SPEED_KPH, FORWARD_FAST_SPEED_KPH);
  TEST_ASSERT_EQUAL_FLOAT(offset, -PWM_ADJUSTMENT_OFFSET * 2);
}

void test_speed_to_pwm_adjustment_if_car_is_faster_than_expected_reverse() {
  desired_direction_value = MOTOR_SPEED_reverse_slow;
  speed_to_pwm_adjustment(REVERSE_SLOW_SPEED_KPH, REVERSE_FAST_SPEED_KPH);
  TEST_ASSERT_EQUAL_FLOAT(offset, PWM_ADJUSTMENT_OFFSET);
  desired_direction_value = MOTOR_SPEED_reverse_slow;
  speed_to_pwm_adjustment(REVERSE_SLOW_SPEED_KPH, REVERSE_FAST_SPEED_KPH);
  TEST_ASSERT_EQUAL_FLOAT(offset, PWM_ADJUSTMENT_OFFSET * 2);
}

void test_speed_to_pwm_adjustment_if_car_is_slower_than_expected_forward() {
  desired_direction_value = MOTOR_SPEED_forward_medium;
  speed_to_pwm_adjustment(FORWARD_FAST_SPEED_KPH, FORWARD_SLOW_SPEED_KPH);
  TEST_ASSERT_EQUAL_FLOAT(offset, PWM_ADJUSTMENT_OFFSET);
  desired_direction_value = MOTOR_SPEED_forward_medium;
  speed_to_pwm_adjustment(FORWARD_FAST_SPEED_KPH, FORWARD_SLOW_SPEED_KPH);
  TEST_ASSERT_EQUAL_FLOAT(offset, PWM_ADJUSTMENT_OFFSET * 2);
}

void test_speed_to_pwm_adjustment_if_car_is_slower_than_expected_reverse() {
  desired_direction_value = MOTOR_SPEED_reverse_slow;
  speed_to_pwm_adjustment(REVERSE_FAST_SPEED_KPH, REVERSE_SLOW_SPEED_KPH);
  TEST_ASSERT_EQUAL_FLOAT(offset, -PWM_ADJUSTMENT_OFFSET);
  desired_direction_value = MOTOR_SPEED_reverse_slow;
  speed_to_pwm_adjustment(FORWARD_FAST_SPEED_KPH, FORWARD_SLOW_SPEED_KPH);
  TEST_ASSERT_EQUAL_FLOAT(offset, -PWM_ADJUSTMENT_OFFSET * 2);
}

void test_speed_to_pwm_adjustment_equal_speeds() {
  desired_direction_value = MOTOR_SPEED_forward_medium;
  speed_to_pwm_adjustment(FORWARD_MEDIUM_SPEED_KPH, FORWARD_MEDIUM_SPEED_KPH);
  TEST_ASSERT_EQUAL_FLOAT(offset, 0.0);
}

void test_speed_to_pwm_adjustment_equal_speeds_under_tolerance() {
  desired_direction_value = MOTOR_SPEED_forward_medium;
  speed_to_pwm_adjustment(FORWARD_MEDIUM_SPEED_KPH + (tolerance / 2), FORWARD_MEDIUM_SPEED_KPH);
  TEST_ASSERT_EQUAL_FLOAT(offset, 0.0);
}