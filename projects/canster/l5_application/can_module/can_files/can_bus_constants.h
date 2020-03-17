#pragma once

#include "can_bus.h"

#define CAN_PORT can1
#define DEBUG 1
#define RAW_DATA_DEBUG 0

#define BAUD_RATE_KBPS 100
#define RX_QUEUE_LEN 100
#define TX_QUEUE_LEN 100

#if DEBUG == 1
#include <stdio.h>
#endif
