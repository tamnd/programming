// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-command-line-arguments/.

#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include <stddef.h>

struct count_args {
    bool verbose;
    int limit;
    char **files;  // points into argv
    size_t nfiles;
};

enum parse_result {
    PARSE_OK = 0,
    PARSE_UNKNOWN_OPTION,
    PARSE_MISSING_VALUE,
    PARSE_BAD_VALUE,
};

// parse_args parses argv[1] to argv[argc - 1] for the tool
// "count [-v] [-n N] [--] [FILE...]" with getopt_long. argv[0] is the
// program name. It prints no message: the caller does that. It uses the
// global state of getopt and resets it first, so it is not thread-safe.
enum parse_result parse_args(int argc, char *argv[], struct count_args *out);

#endif
