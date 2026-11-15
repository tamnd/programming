// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/fizzbuzz-task/.

import assert from "node:assert/strict";
import { test } from "node:test";

import { fizzbuzz } from "./fizzbuzz.js";

const cases = [
  ["fifteen", 15, ["1", "2", "Fizz", "4", "Buzz", "Fizz", "7", "8", "Fizz", "Buzz", "11", "Fizz", "13", "14", "FizzBuzz"]],
  ["thirty", 30, ["1", "2", "Fizz", "4", "Buzz", "Fizz", "7", "8", "Fizz", "Buzz", "11", "Fizz", "13", "14", "FizzBuzz", "16", "17", "Fizz", "19", "Buzz", "Fizz", "22", "23", "Fizz", "Buzz", "26", "Fizz", "28", "29", "FizzBuzz"]],
  ["zero", 0, []],
  ["one", 1, ["1"]],
  ["three", 3, ["1", "2", "Fizz"]],
  ["five", 5, ["1", "2", "Fizz", "4", "Buzz"]],
];

for (const [name, n, want] of cases) {
  test(name, () => {
    assert.deepEqual(fizzbuzz(n), want);
  });
}
