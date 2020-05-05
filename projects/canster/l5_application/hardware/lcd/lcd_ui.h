#pragma once

void lcd_ui__init(void); // first call this in periodic_initialize

// ! IMP run this in the 1hz function only
// * Reason: Contains 20-30ms potential delays in the low-level lcd module, when reading ack/nack/object response from
// lcd screen
void lcd_ui__run_once_1hz(void); // second call this in periodic_cb_1hz