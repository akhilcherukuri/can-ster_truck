#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum { MOTOR_INIT, MOTOR_REVERSE, MOTOR_NEUTRAL, MOTOR_FORWARD, MOTOR_UNDEFINED_STATE } current_motor_state_e;

void motor__init();
void motor__run_10hz(uint32_t);

// void motor__run_state_machine_10hz(current_motor_state_e, uint32_t);