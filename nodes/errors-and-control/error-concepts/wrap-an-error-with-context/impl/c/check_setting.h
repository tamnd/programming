// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-an-error-with-context/.

#ifndef CHECK_SETTING_H
#define CHECK_SETTING_H

#include <stddef.h>

// The codes of the errors of parse_port.
enum setting_code { SETTING_NOT_A_NUMBER = 1, SETTING_OUT_OF_RANGE = 2 };

// An error has a code and a message. C has no chain of causes, so a wrapper
// puts its context in front of the message and keeps the code.
struct setting_error {
    int code;
    char message[256];
};

// parse_port stores the port number of text in *port and returns 0. On an
// error, it fills *err and returns the code.
int parse_port(const char *text, int *port, struct setting_error *err);

// read_port calls parse_port. On an error, it puts "WHERE: read port: " in
// front of the message, keeps the code, and returns it.
int read_port(const char *where, const char *text, int *port, struct setting_error *err);

// check_setting writes "port N", or "error: " and the message, to out, which
// has room for size bytes. It returns the number of bytes that snprintf wanted.
int check_setting(const char *where, const char *text, char *out, size_t size);

#endif
