// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compose-functions/.

/** Returns the composition of the functions. The last one runs first. */
export function compose(...fs) {
  return (x) => fs.reduceRight((acc, f) => f(acc), x);
}

const OPS = new Map([
  ["inc", (x) => x + 1],
  ["dec", (x) => x - 1],
  ["double", (x) => 2 * x],
  // 0 - x and not -x, so that negate(0) is 0 and not -0.
  ["negate", (x) => 0 - x],
]);

/** Composes the functions that ops names and applies the result to x. */
export function composeAll(ops, x) {
  return compose(...ops.map((name) => OPS.get(name)))(x);
}
