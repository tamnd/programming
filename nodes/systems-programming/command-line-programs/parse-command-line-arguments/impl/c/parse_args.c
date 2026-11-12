// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-command-line-arguments/.

#include "parse_args.h"

#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stdlib.h>

// Converts a string of ASCII digits. strtol alone also accepts white space
// and a sign.
static bool decimal(const char *s, int *n) {
    if (*s == '\0') return false;
    for (const char *p = s; *p != '\0'; p++) {
        if (*p < '0' || *p > '9') return false;
    }
    errno = 0;
    long v = strtol(s, nullptr, 10);
    if (errno == ERANGE || v > INT_MAX) return false;
    *n = (int)v;
    return true;
}

enum parse_result parse_args(int argc, char *argv[], struct count_args *out) {
    static const struct option longopts[] = {
        {"verbose", no_argument, nullptr, 'v'},
        {"limit", required_argument, nullptr, 'n'},
        {nullptr, 0, nullptr, 0},
    };
    *out = (struct count_args){.verbose = false, .limit = 10};
    // optind = 0 makes getopt_long start again with the glibc, musl and BSD
    // libraries. opterr = 0 stops the messages on stderr.
    optind = 0;
    opterr = 0;
    int c;
    // The leading ':' makes a missing value return ':' and not '?'.
    while ((c = getopt_long(argc, argv, ":vn:", longopts, nullptr)) != -1) {
        switch (c) {
        case 'v':
            out->verbose = true;
            break;
        case 'n':
            if (!decimal(optarg, &out->limit)) return PARSE_BAD_VALUE;
            break;
        case ':':
            return PARSE_MISSING_VALUE;
        default:
            return PARSE_UNKNOWN_OPTION;
        }
    }
    out->files = argv + optind;
    out->nfiles = (size_t)(argc - optind);
    return PARSE_OK;
}
