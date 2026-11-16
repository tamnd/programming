// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/log-an-error/.

#ifndef READ_PORTS_H
#define READ_PORTS_H

#include <stddef.h>
#include <stdio.h>

// parse_port stores the port number of text in *port and returns 0. On an
// error, it writes the message to message, which has room for size bytes,
// and returns 1 for a text that is not a number or 2 for a value out of range.
int parse_port(const char *text, int *port, char *message, size_t size);

// write_quoted writes value to f in double quotes, and escapes the characters
// that could end the line or the value.
void write_quoted(FILE *f, const char *value);

// read_ports reads the n texts, stores the good ports in ports, which has
// room for n values, and returns their number. It writes a log line to log
// for each bad text, then a summary.
size_t read_ports(const char *const texts[], size_t n, int ports[], FILE *log);

#endif
