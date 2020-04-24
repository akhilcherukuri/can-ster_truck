#pragma once

#include <stdbool.h>

#define GPS_DEBUG 1

typedef struct {
  float latitude;
  float longitude;
} gps_coordinates_s;

void gps__init(void);
void gps__run_once(void);

gps_coordinates_s gps__get_coordinates(void);

bool gps__is_valid(void);

#if GPS_DEBUG == 1
void gps__debug_print_raw_data();
void gps__debug_print_parsed_rmc_data();
#endif
