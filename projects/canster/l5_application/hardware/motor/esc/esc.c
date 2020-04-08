#include "esc.h"

#include "delay.h"
#include "gpio.h"
#include "pwm1.h"

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
static const float FORWARD_SLOW = 15.875;
static const float FORWARD_MEDIUM = 16.0;
static const float FORWARD_FAST = 16.5;
static const float NEUTRAL = 15.0;
static const float REVERSE_SLOW = 14.4;
static const float REVERSE_MEDIUM = 14.25;
static const float REVERSE_FAST = 14.0;

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

/**
 * STATIC FUNCTION DEFINITIONS
 */
static void esc__configure_pwm_channel(void) {
  gpio__construct_with_function(PWM_GPIO_PORT, PWM_GPIO_PIN, PWM_GPIO_FUNCTION);
}

// Helper
static void esc__set_duty_cycle(float duty_cycle) { pwm1__set_duty_cycle(PWM_CHANNEL, duty_cycle); }

/**
 * REFERENCE VALUES
 */
//   Fast (Rev) - 14, Medium (Rev) - 14.25, Slow  (Rev) - 14.5, Neutral - 15
//   Slow (FW) - 15.75, Medium (FW) - 16,  Fast (FW) - 16.5, Fast (FW)- 17, Fast (FW) - 17.5
//   Final Rev - 14.4, Final FW - 15.875