#pragma once

#define GPS_DEBUG 1

void gps__init();

#if GPS_DEBUG == 1

void gps__debug_print_raw_data();

#endif
