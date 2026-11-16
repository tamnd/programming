// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-higher-order-function/.

#ifndef REPEAT_H
#define REPEAT_H

// A pointer to a function from one int to one int.
typedef int (*step_fn)(int);

// C has no closures, so a repeated function is a struct with the function
// and the count.
struct repeated {
    step_fn f;
    int times;
};

// Returns a value that applies f to its argument times times.
struct repeated repeat(step_fn f, int times);

// Applies r to x.
int call_repeated(struct repeated r, int x);

// Returns n / 2 for an even n and 3n + 1 for an odd n.
int collatz_step(int n);

// Returns the value after times Collatz steps from start.
int collatz_after(int start, int times);

#endif
