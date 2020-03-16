#pragma once

#include "can_bus.h"

#define CAN_PORT can1
#define DEBUG 1
#define RAW_DATA_DEBUG 0

#if DEBUG == 1
#include <stdio.h>
#endif