#include "can_driver_led.h"

#include "gpio.h"

const static gpio__port_e GPIO_DRIVER_PORT = GPIO__PORT_0;

const static uint8_t GPIO_MOTOR_PIN = 8;
const static uint8_t GPIO_GEO_PIN = 26;
const static uint8_t GPIO_SENSOR_PIN = 31;

static gpio_s motor_gpio;
static gpio_s geo_gpio;
static gpio_s sensor_gpio;

void can_driver_led__init() {
  motor_gpio = gpio__construct_as_output(GPIO_DRIVER_PORT, GPIO_MOTOR_PIN);
  geo_gpio = gpio__construct_as_output(GPIO_DRIVER_PORT, GPIO_GEO_PIN);
  sensor_gpio = gpio__construct_as_output(GPIO_DRIVER_PORT, GPIO_SENSOR_PIN);
}

// Set
void can_driver_led__set_motor_heartbeat_mia() { gpio__set(motor_gpio); }

void can_driver_led__set_geo_heartbeat_mia() { gpio__set(geo_gpio); }

void can_driver_led__set_sensor_heartbeat_mia() { gpio__set(sensor_gpio); }

// Reset
void can_driver_led__reset_motor_heartbeat_mia() { gpio__reset(motor_gpio); }
void can_driver_led__reset_geo_heartbeat_mia() { gpio__reset(geo_gpio); }
void can_driver_led__reset_sensor_heartbeat_mia() { gpio__reset(sensor_gpio); }
