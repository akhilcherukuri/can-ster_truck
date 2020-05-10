#include "esc.h"

#include "delay.h"
#include "gpio.h"
#include "project.h"
#include "pwm1.h"
#include "rpm.h"

#define PID_FACTOR 0
#define FEEDBACK_LOOP 1
#define DEBUG_FEEDBACK_LOOP 1

/**
 * STATIC VARIABLES
 */
static int16_t desired_direction_value; // -3 to 3
static double desired_speed_kph;
static double offset;
static int16_t old_direction_value;
static float final_adjusted_duty_cycle;

/**
 * STATIC CONST VARIABLES
 */
static const pwm1_channel_e PWM_CHANNEL = PWM1__2_0;

static const gpio__port_e PWM_GPIO_PORT = GPIO__PORT_2;
static const uint8_t PWM_GPIO_PIN = 0;
static const gpio__function_e PWM_GPIO_FUNCTION = GPIO__FUNCTION_1;

/**
 * STATIC CONST FLOAT VARIABLES
 */
static const double PWM_ADJUSTMENT_OFFSET = 0.002;
static const double tolerance = 0.5;

static const float FORWARD_SLOW = 15.875;
static const float FORWARD_MEDIUM = 16.0;
static const float FORWARD_FAST = 16.5;
static const float NEUTRAL = 15.0;
static const float REVERSE_SLOW = 14.4;
static const float REVERSE_MEDIUM = 14.25;
static const float REVERSE_FAST = 14.0;

// No load speeds
static const float FORWARD_SLOW_SPEED_KPH = 5.402647;
static const float FORWARD_MEDIUM_SPEED_KPH = 9.558529;
static const float FORWARD_FAST_SPEED_KPH = 20.779412;
static const float NEUTRAL_SPEED_KPH = 0;
static const float REVERSE_SLOW_SPEED_KPH = 6.233824;
static const float REVERSE_MEDIUM_SPEED_KPH = 10.389706;
static const float REVERSE_FAST_SPEED_KPH = 16.207941;

// Load speeds
static const float FORWARD_MEDIUM_SPEED_KPH_LOAD = 4.2; // 1.17 mps
static const float REVERSE_MEDIUM_SPEED_KPH_LOAD = 2.5; // 0.69 mps

/**
 * STATIC FUNCTIONS
 */
static void esc__configure_pwm_channel(void);

// Helper
static void esc__set_duty_cycle(float duty_cycle);

/**
 * NON-STATIC FUNCTION DEFINITIONS
 */
void esc__init() {
  esc__configure_pwm_channel();
  // pwm1__init_single_edge(100);
  esc__neutral();
  delay__ms(300);
}

void esc__forward_slow() { esc__set_duty_cycle(FORWARD_SLOW); } // Final Forward
void esc__forward_medium() { esc__set_duty_cycle(FORWARD_MEDIUM); }
void esc__forward_fast() { esc__set_duty_cycle(FORWARD_FAST); }
void esc__neutral() { esc__set_duty_cycle(NEUTRAL); }
void esc__reverse_slow() { esc__set_duty_cycle(REVERSE_SLOW); } // Final Reverse
void esc__reverse_medium() { esc__set_duty_cycle(REVERSE_MEDIUM); }
void esc__reverse_fast() { esc__set_duty_cycle(REVERSE_FAST); }

void esc__calibrate_blink_red_once() { esc__set_duty_cycle(20); }
void esc__calibrate_blink_red_twice() { esc__set_duty_cycle(15); }
void esc__calibrate_red_to_green() { esc__set_duty_cycle(10); }

void esc__direction_processor(int16_t direction_value) {
  desired_direction_value = direction_value;
  switch (direction_value) {
  case MOTOR_SPEED_reverse_fast:
    esc__reverse_fast();
    desired_speed_kph = REVERSE_FAST_SPEED_KPH;
    break;
  case MOTOR_SPEED_reverse_medium:
    esc__reverse_medium();
    desired_speed_kph = REVERSE_MEDIUM_SPEED_KPH_LOAD; // REVERSE_MEDIUM_SPEED_KPH;
    break;
  case MOTOR_SPEED_reverse_slow:
    esc__reverse_slow();
    desired_speed_kph = REVERSE_SLOW_SPEED_KPH;
    break;
  case MOTOR_SPEED_neutral:
    esc__neutral();
    desired_speed_kph = NEUTRAL_SPEED_KPH;
    break;
  case MOTOR_SPEED_forward_slow:
    esc__forward_slow();
    desired_speed_kph = FORWARD_SLOW_SPEED_KPH;
    break;
  case MOTOR_SPEED_forward_medium:
    esc__forward_medium();
    desired_speed_kph = FORWARD_MEDIUM_SPEED_KPH_LOAD; // FORWARD_MEDIUM_SPEED_KPH;
    break;
  case MOTOR_SPEED_forward_fast:
    esc__forward_fast();
    desired_speed_kph = FORWARD_FAST_SPEED_KPH;
    break;

  default:
    printf("\nDid not receive direction and speed value");
  }
}

float PID_control(struct state_pid *Pid, float desired_speed, float current_speed) {
  float i_term, p_term, total_term, d_term;
  Pid->kp = 0.02;
  Pid->kd = 0.009;
  Pid->ki = 0.0005;
  Pid->error = desired_speed - current_speed;
  Pid->imax = desired_speed / Pid->ki;
  p_term = Pid->kp * Pid->error;
  if (p_term < 0)
    p_term = 0;
  Pid->istate += Pid->error;
  if (Pid->istate > Pid->imax)
    Pid->istate = Pid->imax;
  if (Pid->istate <= 0)
    Pid->istate = 0;
  i_term = Pid->ki * Pid->istate;
  d_term = Pid->kd * (Pid->last_error - Pid->error);
  Pid->last_error = Pid->error;
  total_term = i_term + p_term + d_term; // new speed the motor should rotate at to compensate the error
  return total_term;
}

float speed_to_pwm_adjustment(double target_speed, double rpm_current_speed) {
  if (desired_direction_value != old_direction_value) {
    old_direction_value = desired_direction_value;
    offset = 0;
  }
  if (desired_direction_value > MOTOR_SPEED_neutral) {
    if ((target_speed - rpm_current_speed > tolerance) || (rpm_current_speed - target_speed > tolerance)) {
      if (rpm_current_speed < target_speed) {
        offset = offset + PWM_ADJUSTMENT_OFFSET;
      } else if (rpm_current_speed > target_speed) {
        offset = offset - PWM_ADJUSTMENT_OFFSET;
      }
    }
  }

  if (desired_direction_value < MOTOR_SPEED_neutral) {
    if ((target_speed - rpm_current_speed > tolerance) || (rpm_current_speed - target_speed > tolerance)) {
      if (rpm_current_speed < target_speed) {
        offset = offset - PWM_ADJUSTMENT_OFFSET;
      } else if (rpm_current_speed > target_speed) {
        offset = offset + PWM_ADJUSTMENT_OFFSET;
      }
    }
  }
  if (desired_direction_value == MOTOR_SPEED_neutral)
    offset = 0;
  return offset;
}

float get_final_pwm() { return final_adjusted_duty_cycle; }

/**
 * STATIC FUNCTION DEFINITIONS
 */
static void esc__configure_pwm_channel(void) {
  gpio__construct_with_function(PWM_GPIO_PORT, PWM_GPIO_PIN, PWM_GPIO_FUNCTION);
}

// Helper
static void esc__set_duty_cycle(float duty_cycle) {
#if FEEDBACK_LOOP == 1
  double current_speed = get_speed_kph();
  double desired_speed = desired_speed_kph;
  float adjusted_duty_cycle = 0.0;
#if PID_FACTOR == 1
  double pid_based_speed = PID_control(&Plantpid, desired_speed, current_speed);
  adjusted_duty_cycle = duty_cycle + (speed_to_pwm_adjustment(pid_based_speed, current_speed)); // Adding offset to pwm
#else
#if DEBUG_FEEDBACK_LOOP == 0
  printf("\nTarget Speed: %f, RPM Current Speed %f", desired_speed, current_speed);
#endif
  adjusted_duty_cycle = duty_cycle + (speed_to_pwm_adjustment(desired_speed, current_speed)); // Adding offset to pwm
#endif

  if (adjusted_duty_cycle > FORWARD_FAST && desired_direction_value > MOTOR_SPEED_neutral) {
    adjusted_duty_cycle = FORWARD_FAST;
  }
  if (adjusted_duty_cycle < REVERSE_FAST && desired_direction_value < MOTOR_SPEED_neutral) {
    adjusted_duty_cycle = REVERSE_FAST;
  }
  final_adjusted_duty_cycle = adjusted_duty_cycle;
  pwm1__set_duty_cycle(PWM_CHANNEL, adjusted_duty_cycle);
#else
  pwm1__set_duty_cycle(PWM_CHANNEL, duty_cycle);
#endif
}

/**
 * REFERENCE VALUES
 */
//   Fast (Rev) - 14, Medium (Rev) - 14.25, Slow  (Rev) - 14.5, Neutral - 15
//   Slow (FW) - 15.75, Medium (FW) - 16,  Fast (FW) - 16.5, Fast (FW)- 17, Fast (FW) - 17.5
//   Final Rev - 14.4, Final FW - 15.875