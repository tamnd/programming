// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-custom-error-type/.

#ifndef CHECK_PORT_H
#define CHECK_PORT_H

#include <stddef.h>

// Why a text is not a valid TCP port number.
enum port_reason { PORT_EMPTY, PORT_NOT_A_NUMBER, PORT_OUT_OF_RANGE };

// C has no exception types, so the error is a struct that the caller passes in.
struct port_error {
    const char *text;         // the text that failed; it points into the caller's string
    enum port_reason reason;
};

// port_reason_name returns the reason as a string literal, such as "empty".
const char *port_reason_name(enum port_reason reason);

// parse_port stores the port number of text in *port and returns 0. On an
// error it fills *err and returns -1.
int parse_port(const char *text, int *port, struct port_error *err);

// check_port writes "port N", or the message of the error, to out, which has
// room for size bytes. It returns the number of bytes that snprintf wanted.
int check_port(const char *text, char *out, size_t size);

#endif
