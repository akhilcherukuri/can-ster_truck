#include "servo.h"

#include <stdio.h>

#include "gpio.h"
#include "pwm1.h"

/**
 * STATIC CONST VARIABLES
 */
static const pwm1_channel_e PWM_CHANNEL = PWM1__2_5;
static const uint32_t PWM_FREQUENCY = 100; // (Hz)

static const gpio__port_e PWM_GPIO_PORT = GPIO__PORT_2;
static const uint8_t PWM_GPIO_PIN = 0;
static const gpio__function_e PWM_GPIO_FUNCTION = GPIO__FUNCTION_1;

static const float HARD_LEFT = 11.0;
static const float SOFT_LEFT = 12.5;
static const float STRAIGHT = 15.0;
static const float SOFT_RIGHT = 17.5;
static const float HARD_RIGHT = 19.0;

/**
 * STATIC FUNCTIONS
 */
static void servo__configure_pwm_channel(void);

static void servo__steer_hard_left(void);
static void servo__steer_hard_right(void);
static void servo__steer_soft_left(void);
static void servo__steer_soft_right(void);
static void servo__steer_straight(void);

// Helper
static void servo__steer_set_duty_cycle(float duty_cycle);

/**
 * NON-STATIC FUNCTION DEFINITIONS
 */
void servo__steer_init(void) {
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
static void servo__configure_pwm_channel(void) {
  gpio__construct_with_function(PWM_GPIO_PORT, PWM_GPIO_PIN, PWM_GPIO_FUNCTION);
}

static void servo__steer_hard_left(void) { servo__steer_set_duty_cycle(HARD_LEFT); }
static void servo__steer_soft_left(void) { servo__steer_set_duty_cycle(SOFT_LEFT); }
static void servo__steer_straight(void) { servo__steer_set_duty_cycle(STRAIGHT); }
static void servo__steer_soft_right(void) { servo__steer_set_duty_cycle(SOFT_RIGHT); }
static void servo__steer_hard_right(void) { servo__steer_set_duty_cycle(HARD_RIGHT); }

// Helper
static void servo__steer_set_duty_cycle(float duty_cycle) { pwm1__set_duty_cycle(PWM_CHANNEL, duty_cycle); }