#pragma once

#define DRIVER 0
#define MOTOR 0
#define SENSOR 1
// ^ Keep Track of who you are as a node

#if DRIVER + MOTOR + SENSOR != 1
#error "Only one node can be active at a time"
#endif