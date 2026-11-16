// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compose-functions/.

/** Returns the composition of the functions. The last one runs first. */
export function compose<T>(...fs: Array<(x: T) => T>): (x: T) => T {
  return (x: T): T => fs.reduceRight((acc: T, f) => f(acc), x);
}

const OPS = new Map<string, (x: number) => number>([
  ["inc", (x) => x + 1],
  ["dec", (x) => x - 1],
  ["double", (x) => 2 * x],
  // 0 - x and not -x, so that negate(0) is 0 and not -0.
  ["negate", (x) => 0 - x],
]);

/** Composes the functions that ops names and applies the result to x. */
export function composeAll(ops: string[], x: number): number {
  const fs = ops.map((name) => {
    const f = OPS.get(name);
    if (f === undefined) {
      throw new Error(`unknown function: ${name}`);
    }
    return f;
  });
  return compose(...fs)(x);
}
