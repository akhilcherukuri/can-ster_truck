#pragma once

#include <stdint.h>
#include <stdio.h>

void esc__init();

void esc__forward_slow();
void esc__forward_medium();
void esc__forward_fast();
void esc__neutral();
void esc__reverse_slow();
void esc__reverse_medium();
void esc__reverse_fast();

void esc__direction_processor(int16_t direction_value);

void esc__calibrate_blink_red_once();
void esc__calibrate_blink_red_twice();
void esc__calibrate_red_to_green();

struct state_pid {
  float istate, imax, imin, kp, ki, kd, last_error, error;
} Plantpid;

float PID_control(struct state_pid *Pid, float desired_speed, float current_speed);

float speed_to_pwm_adjustment(double calculated_speed, double rpm_current_speed);

float get_final_pwm();