#pragma once

typedef void (*bt__read_func_cb)(char *, char *);

void bt__init(void);

/**
 * Calls `bt__read_func_cb` after getting a line and parsing the identifier
 * bt__read_func_cb(sscanf_sanitized_buffer, $_marked_identifier);
 *
 * If NULL is passed function isnt called
 */
void bt__read(bt__read_func_cb read_func);

void bt__write(char *data);
