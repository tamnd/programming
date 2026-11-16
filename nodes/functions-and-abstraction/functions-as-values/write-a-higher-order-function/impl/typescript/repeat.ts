// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-higher-order-function/.

/** A function from one number to one number. */
export type Step = (x: number) => number;

/** Returns a function that applies f to its argument times times. */
export function repeat(f: Step, times: number): Step {
  return (x: number): number => {
    let value = x;
    for (let i = 0; i < times; i++) {
      value = f(value);
    }
    return value;
  };
}

/** Returns n / 2 for an even n and 3n + 1 for an odd n. */
export function collatzStep(n: number): number {
  return n % 2 === 0 ? n / 2 : 3 * n + 1;
}

/** Returns the value after times Collatz steps from start. */
export function collatzAfter(start: number, times: number): number {
  return repeat(collatzStep, times)(start);
}
