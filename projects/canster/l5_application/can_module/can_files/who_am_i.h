#pragma once

#ifndef BOARDS_DEFINED

#define BOARDS_DEFINED
#define BOARD_SENSOR_NODE 0 // USB 0
#define BOARD_DRIVER_NODE 1 // USB 1
#define BOARD_MOTOR_NODE 0  // USB 2

#if BOARD_SENSOR_NODE + BOARD_DRIVER_NODE + BOARD_MOTOR_NODE != 1
#error "Cannot compile since only one Node must be made active during compilation"
#endif

#endif /* BOARDS_DEFINED */