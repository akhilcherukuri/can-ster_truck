#include "rpm.h"

#include <stdio.h>
#include <stdlib.h>

#include "clock.h"
#include "delay.h"
#include "gpio.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"
#include "sys_time.h"

#define RPM_ISR_DEBUG 0
#define RPM_DEBUG 0
#define CIRCUMFERENCE_METER 0.314
#define MPS_TO_KPH_CONVERSION_FACTOR 3.6
#define GEAR_RATIO 2.72

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
static uint16_t pulse_count;
static double speed_kph;

/**
 * STATIC FUNCTIONS
 */
static void rpm__enable_interrupt(void);
static void rpm_sensor_isr_dispatcher(void);
static void rpm__clear_interrupt(void);
static void rpm_sensor_isr_falling(void);
static void rpm_sensor_isr_rising(void);
static void set_pulse_count(uint16_t);
static uint16_t get_pulse_count(void);

/**
 * NON-STATIC FUNCTION DEFINITIONS
 */
void rpm__init() {
  gpio__construct_as_input(RPM_GPIO_PORT, RPM_GPIO_PIN);

  rpm__enable_interrupt();

  lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__GPIO, rpm_sensor_isr_dispatcher, NULL);
  NVIC_EnableIRQ(GPIO_IRQn);
}

float rpm__calculate_speed_kph() {
  uint16_t rotation_per_sec = pulse_count;
#if RPM_DEBUG == 1
  printf("\nPulses: %d ", pulse_count);
#endif
  pulse_count = 0;
  speed_kph = ((CIRCUMFERENCE_METER * rotation_per_sec) * (MPS_TO_KPH_CONVERSION_FACTOR)) / GEAR_RATIO;
#if RPM_DEBUG == 1
  printf("Speed = %lf", speed_kph);
#endif
  return speed_kph;
}

/**
 * STATIC FUNCTION DEFINITIONS
 */
static void rpm__enable_interrupt() {
  LPC_GPIOINT->IO2IntEnF |= (1 << RPM_GPIO_PIN);
  LPC_GPIOINT->IO2IntEnR |= (1 << RPM_GPIO_PIN);
}

static void rpm_sensor_isr_dispatcher(void) {
  if (LPC_GPIOINT->IO2IntStatF & (1 << RPM_GPIO_PIN)) {
    rpm_sensor_isr_falling();
  }

  if (LPC_GPIOINT->IO2IntStatR & (1 << RPM_GPIO_PIN)) {
    rpm_sensor_isr_rising();
  }
  rpm__clear_interrupt();
}

static void rpm__clear_interrupt() { LPC_GPIOINT->IO2IntClr |= (1 << RPM_GPIO_PIN); }

static void rpm_sensor_isr_falling(void) {
  clock_time_at_falling_edge = sys_time__get_uptime_us();
  pulse_count++;
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

static void set_pulse_count(uint16_t pulse) { pulse_count = pulse; }
static uint16_t get_pulse_count(void) { return pulse_count; }