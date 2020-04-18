#include "ultrasonic.h"

#include <stdio.h>

#include "clock.h"
#include "delay.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"
#include "sys_time.h"

/**
 * STATIC CONST VARIABLES
 */
static const double distance_cm_conversion_factor = 57.14;
// static const double distance_inch_conversion_factor = 147.0;

/**
 * STATIC FUNCTIONS
 */
static void ultrasonic__enable_interrupt(uint8_t pin_number);

static void ultrasonic__interrupt_dispatcher(void);
static void ultrasonic__service_echo_fall_rise_isr(ultrasonic_sensor_s *u_sensor);
static void ultrasonic__clear_interrupt(uint8_t pin_number);

static void ultrasonic__enable_trigger_pulse(gpio_s trig_gpio);

/**
 * STATIC VARIABLES
 */
static ultrasonic_sensor_s *u_array[MAX_ULTRASONIC_LENGTH];
static uint8_t u_counter;

/**
 * NON-STATIC FUNCTION DEFINITIONS
 */
void ultrasonic__init(ultrasonic_sensor_s *u_sensor) {
  if (u_counter >= MAX_ULTRASONIC_LENGTH) {
#if ULTRASONIC_DEBUG == 1
    printf("Cannot configure more than %d sensors\r\n", MAX_ULTRASONIC_LENGTH);
#endif
    return;
  }

  gpio__construct_as_input(u_sensor->echo_input.port_number, u_sensor->echo_input.pin_number);
  gpio__construct_as_output(u_sensor->trig_output.port_number, u_sensor->trig_output.pin_number);

  ultrasonic__enable_interrupt(u_sensor->echo_input.pin_number);

  lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__GPIO, ultrasonic__interrupt_dispatcher, NULL);
  u_array[u_counter] = u_sensor;
  u_counter++;
}

/* Note: Attach this to driver_obstacle__obstacle_detected() API */
void ultrasonic__calculate_distance_from_obstacle(ultrasonic_sensor_s *u_sensor) {
  ultrasonic__enable_trigger_pulse(u_sensor->trig_output);
  u_sensor->distance_from_obstacle =
      (double)((u_sensor->clock_time_at_falling_edge - u_sensor->clock_time_at_rising_edge) /
               distance_cm_conversion_factor);

#if ULTRASONIC_DEBUG == 1
  printf("Distance from obstacle = %lf cm\r\n", u_sensor->distance_from_obstacle);
#endif
}

/**
 * STATIC FUNCTION DEFINITIONS
 */
static void ultrasonic__service_echo_fall_rise_isr(ultrasonic_sensor_s *u_sensor) {
  if (LPC_GPIOINT->IO0IntStatF & (1 << u_sensor->echo_input.pin_number)) {
    u_sensor->clock_time_at_falling_edge = sys_time__get_uptime_us();
  }
  if (LPC_GPIOINT->IO0IntStatR & (1 << u_sensor->echo_input.pin_number)) {
    u_sensor->clock_time_at_rising_edge = sys_time__get_uptime_us();
  }
}

static void ultrasonic__enable_interrupt(uint8_t pin_number) {
  LPC_GPIOINT->IO0IntEnF |= (1 << pin_number);
  LPC_GPIOINT->IO0IntEnR |= (1 << pin_number);
}

static void ultrasonic__clear_interrupt(uint8_t pin_number) { LPC_GPIOINT->IO0IntClr |= (1 << pin_number); }

static void ultrasonic__interrupt_dispatcher(void) {
  for (int i = 0; i < MAX_ULTRASONIC_LENGTH; i++) {
    ultrasonic_sensor_s *u_sensor = u_array[i];
    ultrasonic__service_echo_fall_rise_isr(u_sensor);
    ultrasonic__clear_interrupt(u_sensor->echo_input.pin_number);
  }
}

static void ultrasonic__enable_trigger_pulse(gpio_s trig_gpio) {
  gpio__reset(trig_gpio);
  delay__us(2);
  gpio__set(trig_gpio);
  delay__us(10);
  gpio__reset(trig_gpio);
}