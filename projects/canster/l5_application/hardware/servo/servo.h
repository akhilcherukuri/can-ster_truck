#pragma once

void servo__steer_init(void);
void servo__steer_processor(int steering_value); // [-2,2]