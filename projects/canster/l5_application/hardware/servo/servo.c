#include "servo.h"

/**
 * STATIC CONST VARIABLES
 */
static const uint32_t PWM_FREQUENCY = 50; // (Hz)

static const float HARD_LEFT = 6.0;
static const float SOFT_LEFT = 6.75;
static const float STRAIGHT = 7.5;
static const float SOFT_RIGHT = 8.75;
static const float HARD_RIGHT = 9.75;

/**
 * STATIC FUNCTIONS
 */
static void servo__configure_pwm_channel();

static void servo__steer_hard_left(void);

static void servo__steer_hard_right(void);

static void servo__steer_soft_left(void);

static void servo__steer_soft_right(void);

static void servo__steer_straight(void);

/**
 * NON-STATIC FUNCTION DEFINITIONS
 */
void servo__steer_init() {
  pwm1__init_single_edge(PWM_FREQUENCY);
  servo__configure_pwm_channel();
}

void servo__steer_processor(int steering_value) {
  switch (steering_value) {
  case -2:
    servo__steer_hard_left();
    break;
  case -1:
    servo__steer_soft_left();
    break;
  case 0:
    servo__steer_straight();
    break;
  case 1:
    servo__steer_soft_right();
    break;
  case 2:
    servo__steer_hard_right();
    break;
  default:
    printf("Did not receive steering value");
  }
}

/**
 * STATIC FUNCTION DEFINITIONS
 */
static void servo__configure_pwm_channel() { gpio__construct_with_function(GPIO__PORT_2, 0, GPIO__FUNCTION_1); }

static void servo__steer_hard_left() { pwm1__set_duty_cycle(PWM1__2_0, HARD_LEFT); }

static void servo__steer_soft_left() { pwm1__set_duty_cycle(PWM1__2_0, SOFT_LEFT); }

static void servo__steer_straight() { pwm1__set_duty_cycle(PWM1__2_0, STRAIGHT); }

static void servo__steer_soft_right() { pwm1__set_duty_cycle(PWM1__2_0, SOFT_RIGHT); }

static void servo__steer_hard_right() { pwm1__set_duty_cycle(PWM1__2_0, HARD_RIGHT); }