// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-a-subprocess/.

#ifndef RUN_H
#define RUN_H

#include <stddef.h>

// The result of a child process. out and err end with a NUL byte that is not
// part of the length. Free them with run_result_free.
struct run_result {
    int status;    // the exit status, 0 to 255, or -1 when signal is not 0
    int signal;    // the signal that stopped the child, or 0
    char *out;     // standard output
    size_t out_len;
    char *err;     // standard error
    size_t err_len;
};

// run starts argv[0] with the arguments argv, which ends with a null pointer.
// It searches PATH for argv[0] and starts no shell. The standard input of the
// child is /dev/null. run reads both output streams to the end, waits for the
// child and returns 0. On an error it returns -1 and sets errno; ENOENT means
// that the program was not found.
int run(char *const argv[], struct run_result *result);

void run_result_free(struct run_result *result);

#endif
