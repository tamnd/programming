// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/fizzbuzz-task/.

#ifndef FIZZBUZZ_H
#define FIZZBUZZ_H

// fizzbuzz returns a new array of n new strings: FizzBuzz, Fizz, Buzz or the
// number for each number from 1 to n. The caller frees each string and the
// array with fizzbuzz_free. It returns NULL when malloc fails.
char **fizzbuzz(int n);

// fizzbuzz_free frees the n strings and the array that fizzbuzz returned.
void fizzbuzz_free(char **out, int n);

#endif
