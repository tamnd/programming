// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/start-a-thread/.

#ifndef GREETINGS_H
#define GREETINGS_H

#include <stddef.h>

// Starts one thread for each of the n names. Thread i builds "hello from " followed by names[i].
// Returns a new array of n new strings, or NULL when a thread or the memory cannot be created.
char **thread_greetings(const char *const names[], size_t n);

// Frees the array that thread_greetings returned.
void free_greetings(char **greetings, size_t n);

#endif
