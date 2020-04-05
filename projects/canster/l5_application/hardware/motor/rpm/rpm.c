#include "rpm.h"

#include "gpio.h"

#include "clock.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"
#include "sys_time.h"
#include <stdio.h>
#include <stdlib.h>

#include "delay.h"

#define RPM_ISR_DEBUG 0
#define circumference 0.33

/**
 * STATIC VARIABLES
 */
static uint32_t clock_time_at_rising_edge, clock_time_at_falling_edge;

/**
 * STATIC CONST VARIABLES
 */
static const gpio__port_e RPM_GPIO_PORT = GPIO__PORT_2;
static const uint8_t RPM_GPIO_PIN = 2;

/**
 * GLOBAL VARIABLES
 */
uint16_t pulse = 0;
double speed = 0; // kmph

/**
 * STATIC FUNCTIONS
 */
static void rpm__enable_interrupt(void);
static void rpm_sensor_isr_dispatcher(void);
static void rpm__clear_interrupt(void);
static void rpm_sensor_isr_falling(void);
static void rpm_sensor_isr_rising(void);

/**
 * NON-STATIC FUNCTION DEFINITIONS
 */
void rpm__init() {
  gpio__construct_as_input(RPM_GPIO_PORT, RPM_GPIO_PIN);

  rpm__enable_interrupt();

  lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__GPIO, rpm_sensor_isr_dispatcher, NULL);
  NVIC_EnableIRQ(GPIO_IRQn);
}

float rpm__calculate_speed() {
  uint16_t rotation_per_sec = pulse;
  printf("Pulses: %d", pulse);
  pulse = 0;
  speed = (circumference * rotation_per_sec) / (2.7);
  printf("\nSpeed = %lf", speed);
  return speed;
}

/**
 * STATIC FUNCTION DEFINITIONS
 */
static void rpm__enable_interrupt() {
  LPC_GPIOINT->IO2IntEnF |= (1 << 2);
  LPC_GPIOINT->IO2IntEnR |= (1 << 2);
}

static void rpm_sensor_isr_dispatcher(void) {

  if (LPC_GPIOINT->IO2IntStatF & (1 << 2)) {
    rpm_sensor_isr_falling();
  }

  if (LPC_GPIOINT->IO2IntStatR & (1 << 2)) {
    rpm_sensor_isr_rising();
  }
  rpm__clear_interrupt();
}

static void rpm__clear_interrupt() { LPC_GPIOINT->IO2IntClr |= (1 << 2); }

static void rpm_sensor_isr_falling(void) {
  clock_time_at_falling_edge = sys_time__get_uptime_us();
  pulse++;
#if RPM_ISR_DEBUG == 1
  fprintf(stderr, "Falling edge, %lu\n", clock_time_at_falling_edge);
#endif
}

static void rpm_sensor_isr_rising(void) {
  clock_time_at_rising_edge = sys_time__get_uptime_us();
#if RPM_ISR_DEBUG == 1
  fprintf(stderr, "Rising edge, %lu\n", clock_time_at_rising_edge);
#endif
}